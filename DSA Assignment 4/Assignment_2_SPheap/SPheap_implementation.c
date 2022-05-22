#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <math.h>
#include <limits.h>

#include "SPheap_header.h"

#define m 28

ASL_element ASL[2 * m];
char *CMD;
void *base_address;
size_t available_memory;
float internal_fragment;
int no_splits;
int no_merges;
int no_alloc_req;
int no_dealloc_req;
parent_list *head = NULL;

void *init_memory()
{
    for (int i = 0; i < 2 * m; i++)
    {
        ASL[i].head = NULL;
        ASL[i].tail = NULL;
        if (i % 2)
            ASL[i].size = 1 << ((i + 1) / 2);
        else
        {
            ASL[i].size = 3 * (1 << (i / 2 - 1));
            if (i == 0)
                ASL[i].size = 1;
        }
    }
    base_address = malloc(1 << m);
    if (base_address == NULL)
        memory_not_available();
    ASL_node *base_block = create_ASL_node(base_address);
    ASL[2 * m - 1].head = base_block;
    ASL[2 * m - 1].tail = base_block;
    available_memory = 1 << m;
    internal_fragment = 0;
    CMD = (char *)calloc(1 << m, 1);
    if (CMD == NULL)
        memory_not_available();
    set_tag(0, 0);
    set_kval(0, m);
    set_type(0, 0);
    no_splits = 0;
    no_merges = 0;
    no_alloc_req = 0;
    no_dealloc_req = 0;
}

void memory_not_available()
{
    printf("memory not available");
    exit(1);
}

ASL_node *create_ASL_node(void *address)
{
    ASL_node *temp = (ASL_node *)malloc(sizeof(ASL_node));
    if (temp == NULL)
        memory_not_available();
    temp->address = address;
    temp->next = NULL;
    temp->previous = NULL;
    return temp;
}

void insert_ASL_node(int index, ASL_node *node)
{
    if (ASL[index].tail)
    {
        ASL[index].tail->next = node;
        node->previous = ASL[index].tail;
        ASL[index].tail = node;
    }
    else
    {
        ASL[index].tail = node;
        ASL[index].head = node;
    }
}

void *delete_ASL_node(int index)
{
    ASL_node *t;
    void *address;
    ASL_node *temp = ASL[index].head;
    void *source_address = temp->address;
    size_t offset = (char *)source_address - (char *)base_address;
    ASL[index].head = temp->next;
    if (temp->next)
        temp->next->previous = NULL;
    else
        ASL[index].tail = NULL;
    address = temp->address;
    free(temp);
    set_tag(offset, 1);
    return address;
}

void set_type(size_t offset, int type)
{
    if (offset > (1 << m) || offset < 0)
        return;
    CMD[offset] = (CMD[offset] & (0xFC)) | (type);
}

int get_type(size_t offset)
{
    if (offset > (1 << m) || offset < 0)
        return -1;
    return (int)(CMD[offset] & (0x03));
}

void set_kval(size_t offset, int kval)
{
    if (offset > (1 << m) || offset < 0)
        return;
    CMD[offset] = (CMD[offset] & (0x83)) | (kval << 2);
}

int get_kval(size_t offset)
{
    if (offset > (1 << m) || offset < 0)
        return -1;
    return (int)(CMD[offset] >> 2 & (0x1f));
}

void set_tag(size_t offset, int tag)
{
    if (offset > (1 << m) || offset < 0)
        return;
    CMD[offset] = (CMD[offset] & (0x7F)) | (tag << 7);
}

int get_tag(size_t offset)
{
    if (offset > (1 << m) || offset < 0)
        return -1;
    return (int)(CMD[offset] >> 7 & (0x01));
}

void set_parent_type(size_t address, size_t size)
{
    parent_list *temp;
    temp = (parent_list *)malloc(sizeof(parent_list));
    temp->address = address;
    temp->size = size;
    temp->parent_type = get_type(address);
    temp->next = head;
    head = temp;
}

int get_parent_type(size_t address, size_t size)
{
    parent_list *temp = head;
    parent_list *prev = NULL;
    while (temp != NULL && (temp->address != address || temp->size != size))
    {
        prev = temp;
        temp = temp->next;
    }
    if (temp == NULL)
    {
        printf("not found an entry in list parent type\n");
        return 0;
    }
    if (prev)
        prev->next = temp->next;
    else
        head = head->next;
    int type = temp->parent_type;
    free(temp);
    return type;
}

void *get_base_address()
{
    return base_address;
}

float get_internal_fragment()
{
    return internal_fragment;
}

int get_split_count()
{
    return no_splits;
}

int get_merge_count()
{
    return no_merges;
}

size_t get_memory_available_currently()
{
    return available_memory;
}

size_t get_total_available_memory()
{
    return (1 << m);
}

int split(int index)
{
    int temp_index = index;
    int split_count = 0;
    if (index == 0 && ASL[1].head != NULL)
    {
        temp_index = 1;
    }
    while (temp_index < 2 * m - 1 && ASL[++temp_index].head == NULL)
    {
    }

    if (ASL[temp_index].head == NULL)
    {
        if (index == 0 && ASL[1].head != NULL)
        {
            void *address = delete_ASL_node(1);
            ASL_node *temp = create_ASL_node(address);
            insert_ASL_node(0, temp);
            available_memory = available_memory - 1;
            return 1;
        }
        return 0;
    }
    while (ASL[index].head == NULL)
    {
        no_splits++;
        if (ASL[temp_index].size % 3 == 0)
        {
            split_helper(1, 2, 3, &temp_index, index);
        }
        else
        {
            split_helper(3, 3, 1, &temp_index, index);
        }
    }
    return 1;
}

void split_helper(int type1, int type2, int c, int *temp_index, int index)
{
    int kval_source;
    int index2;
    void *source_address;
    size_t temp1_offset, temp2_offset;
    ASL_node *temp1, *temp2;
    kval_source = log2(ASL[*temp_index].size / c);
    source_address = delete_ASL_node(*temp_index);
    temp1_offset = (char *)source_address - (char *)base_address;
    if (c == 1)
        set_parent_type(temp1_offset, ASL[*temp_index].size);
    temp2_offset = temp1_offset + (3 / c) * (1 << (kval_source + (3 * c - 7) / 2));
    set_tag(temp1_offset, 0);
    set_tag(temp2_offset, 0);
    set_kval(temp1_offset, kval_source + (3 * c - 7) / 2);
    set_kval(temp2_offset, kval_source + (3 - c) * (-1));
    set_type(temp1_offset, type1);
    set_type(temp2_offset, type2);
    temp1 = create_ASL_node(source_address);
    temp2 = create_ASL_node(temp2_offset + (char *)base_address);
    insert_ASL_node(*temp_index - 1, temp1);
    index2 = *temp_index - (9 - c) / 2;
    if (index2 < 0)
        index2 = 0;
    insert_ASL_node(index2, temp2);
    if (ASL[index2].size > ASL[index].size && ASL[index2].size != 2)
        *temp_index = index2;
    else
        (*temp_index)--;
}

size_t search_buddy(size_t offset)
{
    int type = get_type(offset);
    int kval = get_kval(offset);
    size_t buddy_offset;
    size_t new_offset;
    if (type == 3)
    {
        if (offset % (int)pow(2, kval + 2) == 0)
        {
            buddy_offset = offset + 3 * (int)(1 << kval);
            if (get_tag(buddy_offset) == 0 && get_kval(buddy_offset) == kval && get_type(buddy_offset) == 3)
            {
                delete_buddy(buddy_offset, 1, kval);
                return merge(offset, buddy_offset, kval + 2, 1);
            }
        }
        else
        {
            buddy_offset = offset - 3 * (int)(1 << kval);
            if (get_tag(buddy_offset) == 0 && get_kval(buddy_offset) == kval && get_type(buddy_offset) == 3)
            {
                delete_buddy(buddy_offset, 3, kval);
                return merge(offset, buddy_offset, kval + 2, 1);
            }
        }
    }
    else if (type == 1)
    {
        buddy_offset = offset + (int)(1 << kval);
        if (get_tag(buddy_offset) == 0 && get_kval(buddy_offset) == kval - 1 && get_type(buddy_offset) == 2)
        {
            delete_buddy(buddy_offset, 1, kval - 1);
            return merge(offset, buddy_offset, kval - 1, 3);
        }
    }
    else if (type == 2)
    {
        buddy_offset = offset - (int)(1 << (kval + 1));
        if (get_tag(buddy_offset) == 0 && get_kval(buddy_offset) == kval + 1 && get_type(buddy_offset) == 1)
        {
            delete_buddy(buddy_offset, 1, kval + 1);
            return merge(offset, buddy_offset, kval, 3);
        }
    }
    return INT_MAX;
}

void delete_buddy(size_t buddy_offset, int c, int kval)
{
    int index;
    if (c == 3)
    {
        index = 2 * kval + 2;
    }
    else
        index = 2 * kval - 1;
    ASL_node *temp = ASL[index].head;
    ASL_node *prev = NULL;
    while (temp != NULL && temp->address != buddy_offset + (char *)base_address)
    {
        prev = temp;
        temp = temp->next;
    }
    if (temp == NULL)
    {
        printf("something is wrong, buddy is not in the given index\n");
        exit(1);
    }
    if (prev == NULL)
    {
        ASL[index].head = temp->next;
    }
    else
        prev->next = temp->next;

    if (temp->next)
    {
        temp->next->previous = prev;
    }
    else
        ASL[index].tail = prev;
    free(temp);
}

size_t merge(size_t temp_offset, size_t buddy_offset, int kval, int c)
{
    no_merges++;
    size_t new_offset;
    if (temp_offset > buddy_offset)
        new_offset = buddy_offset;
    else
        new_offset = temp_offset;
    int type = get_type(buddy_offset);
    if (type == 3)
    {
        set_type(new_offset, get_parent_type(new_offset, c * (1 << kval)));
    }
    else
        set_type(new_offset, 3);
    set_kval(new_offset, kval);
    return new_offset;
}

void *malloc_SPheap(size_t no_bytes)
{
    no_alloc_req++;
    void *address;
    if (no_bytes > (1 << m) || no_bytes <= 0)
    {
        printf("memory cannot be allocated,total available memory is %zu bytes\n", available_memory);
        return NULL;
    }
    int index;
    if (no_bytes == 1)
        index = 0;
    else
    {
        index = 2 * ceil(log2(no_bytes)) - 1;
        if (ASL[index - 1].size >= no_bytes)
            index = index - 1;
    }

    if (ASL[index].head)
    {
        available_memory = available_memory - ASL[index].size;
        internal_fragment += (float)(ASL[index].size - no_bytes);
        address = delete_ASL_node(index);
        return address;
    }
    else
    {
        int available_flag = split(index);
        if (!available_flag)
        {
            printf("memory cannot be allocated,total available memory is %zu bytes\n", available_memory);
            return NULL;
        }
        available_memory = available_memory - ASL[index].size;
        internal_fragment += (float)(ASL[index].size - no_bytes);
        address = delete_ASL_node(index);
        return address;
    }
}

int free_SPheap(void *address)
{
    no_dealloc_req++;
    size_t new_offset;
    size_t t_offset = (char *)address - (char *)base_address;
    int kval = get_kval(t_offset);
    if (get_type(t_offset) == 3 && t_offset % (int)(1 << (kval + 2)) == 0)
    {
        available_memory = available_memory + 3 * (int)(1 << kval);
    }
    else
        available_memory = available_memory + (int)(1 << kval);

    if (get_tag(t_offset) != 1)
    {
        printf("node freed already or an invalid address\n");
        return 0;
    }
    ASL_node *newnode;
    do
    {
        new_offset = t_offset;
        t_offset = search_buddy(t_offset);

    } while (t_offset != INT_MAX);
    newnode = create_ASL_node(new_offset + (char *)base_address);
    kval = get_kval(new_offset);
    set_tag(new_offset, 0);
    if (get_type(new_offset) != 3 || new_offset % (int)(1 << (kval + 2)) != 0)
    {
        insert_ASL_node(2 * kval - 1, newnode);
    }
    else
        insert_ASL_node(2 * kval + 2, newnode);
    return 1;
}

void print_SPheap_stats()
{
    printf("\nNumber of allocation requests : %d", no_alloc_req);
    printf("\nNumber of deallocation requests : %d", no_dealloc_req);
    printf("\nNumber of area splits : %d", get_split_count());
    printf("\nNumber of buddy recombinations : %d", get_merge_count());
}
