#ifndef SPHEAP_HEADER_H
#define SPHEAP_HEADER_H

typedef struct ASL_node
{
    void *address;
    struct ASL_node *next;
    struct ASL_node *previous;
} ASL_node;

typedef struct ASL_element
{
    size_t size;
    ASL_node *head;
    ASL_node *tail;
} ASL_element;

typedef struct parent_list
{
    size_t address;
    int parent_type;
    size_t size;
    struct parent_list *next;
} parent_list;

void *init_memory();
void memory_not_available();
ASL_node *create_ASL_node(void *address);
void insert_ASL_node(int index, ASL_node *node);
void *delete_ASL_node(int index);
void set_type(size_t offset, int type);
int get_type(size_t offset);
void set_kval(size_t offset, int kval);
int get_kval(size_t offset);
void set_tag(size_t offset, int tag);
int get_tag(size_t offset);
void set_parent_type(size_t address, size_t size);
int get_parent_type(size_t address, size_t size);
void *get_base_address();
float get_internal_fragment();
int get_split_count();
int get_merge_count();
size_t get_memory_available_currently();
size_t get_total_available_memory();
int split(int index);
void split_helper(int type1, int type2, int c, int *temp_index, int index);
size_t search_buddy(size_t offset);
void delete_buddy(size_t offset, int c, int kval);
size_t merge(size_t temp_offset, size_t buddy_offset, int kval, int c);
void *malloc_SPheap(size_t no_bytes);
int free_SPheap(void *address);
void print_SPheap_stats();

#endif