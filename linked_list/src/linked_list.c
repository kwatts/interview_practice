#include "linked_list.h"

#include <stdlib.h>

int removeValue(Node** ppHead, int value)
{
  while (*ppHead) {
    if ((*ppHead)->value != value) {
      ppHead = &(*ppHead)->next;
      continue;
    } 

    Node *next_node = (*ppHead)->next;
    free(*ppHead);
    *ppHead = next_node;
    return 1;
  }
  
  return 0;
}
