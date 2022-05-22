#include <stdio.h>
#include <stdlib.h>
#include "SPheap_header.h"
#include <math.h>
#include <limits.h>

typedef struct node
{
    int digit;
    struct node *next;
} node;

typedef struct list
{
    node *head;
    int len;
} list;

int total_bytes_req = 0;

void create_list(list *num);
void insert_beg(list *num, int digit);
void print_number(list *num);
void append(list *num, int digit);
int get_digit(list *num, int pos);
int read_line(char *expr, int len);
int check_zero(list *num);
list *add_numbers(list *num1, list *num2);
list *multiply_numbers(list *num1, list *num2);
list *evaluate_infix(char *expr);
void delete_list(list *num);

void create_list(list *num)
{
    num->head = NULL;
    num->len = 0;
}
void insert_beg(list *num, int digit)
{
    node *temp;
    temp = (node *)malloc_SPheap(sizeof(node));
    total_bytes_req += sizeof(node);
    temp->digit = digit;
    temp->next = NULL;
    if (num->head == NULL)
    {
        num->head = temp;
    }
    else
    {
        temp->next = num->head;
        num->head = temp;
    }
    num->len++;
    return;
}
void print_number(list *num)
{
    node *p = num->head;
    while (p != NULL)
    {
        printf("%d", p->digit);
        if (p->next != NULL)
            printf(",");
        else
            printf("$");
        p = p->next;
    }
}
void append(list *num, int digit)
{
    node *temp, *p;
    temp = (node *)malloc_SPheap(sizeof(node));
    total_bytes_req += sizeof(node);
    temp->digit = digit;
    temp->next = NULL;
    if (num->head == NULL)
    {
        num->head = temp;
    }
    else
    {
        node *p = num->head;
        while (p->next != NULL)
            p = p->next;
        p->next = temp;
    }
    num->len++;
    return;
}
int get_digit(list *num, int pos)
{
    node *p;
    p = num->head;
    for (int i = 0; i < pos; i++)
        p = p->next;
    return p->digit;
}
int read_line(char *expr, int len)
{
    int i = 0;
    int ch;
    while ((ch = getchar()) != '\n' && i < len - 1)
    {
        expr[i] = ch;
        i++;
    }
    expr[i] = '\0';
    return i;
}
int check_zero(list *num)
{
    int i, flag = 0;
    for (i = 0; i < num->len; i++)
    {
        if (get_digit(num, i) != 0)
            flag = 1;
    }
    return flag;
}
list *add_numbers(list *num1, list *num2)
{
    list *sum;
    sum = (list *)malloc_SPheap(sizeof(list));
    total_bytes_req += sizeof(list);
    create_list(sum);
    int digit = 0, i, j;
    i = num1->len;
    j = num2->len;
    int carry = 0;
    while (i != 0 || j != 0)
    {
        if (i == 0)
        {
            digit = get_digit(num2, j - 1) + carry;
            j--;
            carry = digit / 1000;
            digit = digit % 1000;
            insert_beg(sum, digit);
        }
        else if (j == 0)
        {
            digit = get_digit(num1, i - 1) + carry;
            i--;
            carry = digit / 1000;
            digit = digit % 1000;
            insert_beg(sum, digit);
        }
        else
        {
            digit = get_digit(num1, i - 1) + get_digit(num2, j - 1) + carry;
            i--;
            j--;
            carry = digit / 1000;
            digit = digit % 1000;
            insert_beg(sum, digit);
        }
    }

    if (carry != 0)
        insert_beg(sum, carry);
    return sum;
}
list *multiply_numbers(list *num1, list *num2)
{
    list *product;
    product = (list *)malloc_SPheap(sizeof(list));
    total_bytes_req += sizeof(list);
    create_list(product);
    if ((check_zero(num1) == 0) || (check_zero(num2) == 0))
    {
        insert_beg(product, 0);
        return product;
    }
    int x = num1->len - num2->len;
    if (x < 0)
    {
        product = multiply_numbers(num2, num1);
        return product;
    }
    else
    {
        int n = num2->len - 1, j, i;
        int a = num2->len, b = num1->len;
        int tmp[2 * b];
        for (i = 0; i < 2 * b; i++)
            tmp[i] = 0;
        int n1 = 0, n2 = 0;
        int k = 2 * b - 1;
        for (i = 0; i < a; i++)
        {
            int m = num1->len - 1;
            int carry1 = 0, carry2 = 0;
            for (j = k - i; j > b - 2; j--)
            {
                if (m != -1 && n != -1)
                {
                    n1 = get_digit(num1, m) * get_digit(num2, n) + carry1;
                    m--;
                    carry1 = n1 / 1000;
                    n1 = n1 % 1000;
                    n2 = tmp[j] + n1 + carry2;
                    carry2 = n2 / 1000;
                    n2 = n2 % 1000;
                    tmp[j] = n2;
                }
                else
                {
                    break;
                }
            }
            tmp[j] = carry1 + carry2 + tmp[j];
            b--;
            n--;
        }
        j = 0;
        for (i = 2 * num1->len - 1; i > b - 1 && i >= 0; i--)
        {
            insert_beg(product, tmp[i]);
            j++;
        }
        return product;
    }
}
void delete_list(list *num)
{
    while (num->head != NULL)
    {
        node *p = num->head;
        num->head = p->next;
        free_SPheap(p);
    }
}
list *evaluate_infix(char *expr)
{
    int i = 0, digit, j, k, d;
    list *x, *y;
    char op = '=';
    x = (list *)malloc_SPheap(sizeof(list));
    y = (list *)malloc_SPheap(sizeof(list));
    total_bytes_req +=  2* sizeof(list);
    create_list(x);
    create_list(y);
    k = 0;
    while (1)
    {
        switch (expr[i])
        {
        case '0':
        case '1':
        case '2':
        case '3':
        case '4':
        case '5':
        case '6':
        case '7':
        case '8':
        case '9':
            i++;
            if (expr[i + 2] == '$')
            {
                digit = 0;
                for (j = i - 1; j < i + 2; j++)
                {
                    d = expr[j] - '0';
                    digit = digit * 10 + d;
                }
                if (k == 0)
                    append(x, digit);
                else if (k == 1)
                    append(y, digit);
                else
                {
                    return NULL;
                }
                i = i + 2;
            }
            break;
        case ',':
            digit = 0;
            for (j = i - 3; j < i; j++)
            {
                d = expr[j] - '0';
                digit = digit * 10 + d;
            }
            if (k == 0)
                append(x, digit);
            else if (k == 1)
                append(y, digit);
            else
            {
                return NULL;
            }
            i++;
            break;
        case ' ':
            i++;
            break;
        case '=':
            switch (op)
            {
            case '+':
                if (x != NULL && y != NULL)
                    x = add_numbers(x, y);
                k = 1;
                delete_list(y);
                y = NULL;
                y = (list *)malloc_SPheap(sizeof(list));
                total_bytes_req += sizeof(list);
                create_list(y);
                op = '=';
                break;
            case '*':
                if (x != NULL && y != NULL)
                    x = multiply_numbers(x, y);
                k = 1;
                delete_list(y);
                y = NULL;
                y = (list *)malloc_SPheap(sizeof(list));
                total_bytes_req += sizeof(list);
                create_list(y);
                op = '=';
                break;
            case '=':
                break;
            default:
                return NULL;
                break;
            }
            i++;
            break;
        case '$':
            k++;
            if (k == 2)
            {
                switch (op)
                {
                case '+':
                    if (x != NULL && y != NULL)
                        x = add_numbers(x, y);
                    k = 1;
                    delete_list(y);
                    y = NULL;
                    y = (list *)malloc_SPheap(sizeof(list));
                    total_bytes_req += sizeof(list);
                    create_list(y);
                    op = '=';
                    break;
                case '*':
                    if (x != NULL && y != NULL)
                        x = multiply_numbers(x, y);
                    k = 1;
                    delete_list(y);
                    y = NULL;
                    y = (list *)malloc_SPheap(sizeof(list));
                    total_bytes_req += sizeof(list);
                    create_list(y);
                    op = '=';
                    break;
                case '=':
                    break;
                default:
                    return NULL;
                    break;
                }
            }
            i++;
            break;
        case '+':
            op = '+';
            i++;
            break;
        case '*':
            op = '*';
            i++;
            break;
        case '\0':
            return x;

        default:
            return NULL;
        }
    }
    return x;
}
int main()
{
    init_memory(sizeof(list));
    char expr[2048];
    int x;
    list *result;
    result = (list *)malloc_SPheap(sizeof(list));
    total_bytes_req += sizeof(list);
    create_list(result);
    printf("For examples of input format see the README file\nEnter = to terminate the program\nEnter infix string for evaluation\n");

    while ((x = read_line(expr, 2048)))
    {
        if (expr[0] == '=' && expr[1] == '\0')
            break;
        result = evaluate_infix(expr);
        if (result == NULL)
        {
            fprintf(stderr, "Error in expression\n");
        }
        else
        {
            print_number(result);
            printf("\n");
            print_SPheap_stats();
            printf("\n");
        }
    }
    return 0;
}
