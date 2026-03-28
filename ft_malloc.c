#include "ft_malloc.h"
#include <stdlib.h>

t_list_gc **get_manger(void) {
  static t_list_gc *gc = NULL;
  return (&gc);
}

void *ft_malloc(size_t size) {
  void *ptr = malloc(size);
  if (!ptr)
    return (NULL);
  t_list_gc *new = malloc(sizeof(t_list_gc));
  if (!new) {
    free(ptr);
    return (NULL);
  }
  new->address = ptr;
  new->next = *get_manger();
  *get_manger() = new;
  return (ptr);
}

void free_all(void) {
  t_list_gc *curr = *get_manger();
  while (curr) {
    t_list_gc *next = curr->next;
    free(curr->address);
    free(curr);
    curr = next;
  }
  *get_manger() = NULL;
}

void ft_panic(void *ptr) {
  if (ptr)
    free(ptr);
  free_all();
  exit(1);
}

void ft_free(void *ptr) {
  if (!ptr)
    return;
  t_list_gc *curr = *get_manger();
  t_list_gc *prev = NULL;
  while (curr) {
    if (curr->address == ptr) {
      if (prev)
        prev->next = curr->next;
      else
        *get_manger() = curr->next;
      free(curr->address);
      free(curr);
      return;
    }
    prev = curr;
    curr = curr->next;
  }
}
