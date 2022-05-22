#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include <memory.h>

#include "SPheap_header.h"

typedef struct list_node
{
  int time;
  void *address;
  struct list_node *next;
} list_node;

list_node *list_head = NULL;
list_node *prev;
list_node *ptr1;

int time_step = 0;
int total_bytes_req = 0;

size_t uniform_distribution_request(int lb, int ub)
{
  size_t out;
  out = (rand() % (ub - lb + 1)) + lb;
  return out;
}

size_t truncated_exponential_distribution_request(int lb, int ub, int mean)
{
  float u = (float)rand() / RAND_MAX;
  float p = lb + (-1 * mean * log(1 - u));
  if (p > ub)
  {
    return ub;
  }
  else
    return p;
}

list_node *add_node(int time, void *address, list_node *head)
{
  list_node *newnode = (list_node *)malloc(sizeof(list_node));
  newnode->next = head;
  newnode->time = time;
  newnode->address = address;

  return newnode;
}

void main()
{
  int choice = 1;
  init_memory();
  srand(time(0));
  printf("Press\n1 for generating uniform request\n2 for generating exponential request\n");
  scanf("%d", &choice);
  while (1)
  {
    time_step++;
    size_t request_size;
    if (choice == 2)
      request_size = truncated_exponential_distribution_request(100, 2000, 1000);
    else
      request_size = uniform_distribution_request(100, 2000);

    size_t time = uniform_distribution_request(1, 100);
    void *address = malloc_SPheap(request_size);
    if (address != NULL)
    {
      total_bytes_req += request_size;
      list_node *newnode = add_node(time + time_step, address, list_head);
      list_head = newnode;
      prev = list_head;
      for (ptr1 = list_head; ptr1 != NULL; ptr1 = ptr1->next)
      {
        if (((ptr1->time) == time_step)&&(time_step<=2000))
        {
          free_SPheap(ptr1->address);
          if (ptr1 == list_head)
          {
            list_head = ptr1->next;
            prev = ptr1->next;
          }
          else
            prev->next = ptr1->next;
          free(ptr1);
        }
        else
          prev = ptr1;
      }
    }
    else
    {
      printf("external fragmentation : %f\n", (float)((float)get_memory_available_currently() / (float)get_total_available_memory()));
      printf("allocation request was of size %zu bytes\n", request_size);
      exit(1);
    }

    if (time_step % 200 == 0)
    {
      printf("\nTime step %d", time_step);
      print_SPheap_stats();
      printf("\nInternal fragmentation/total bytes requested : %f\n", (get_internal_fragment()) / total_bytes_req);
    }
  }
}
