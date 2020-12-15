#ifndef LINKED_LIST_PRACTICE
#define LINKED_LIST_PRACTICE

typedef struct node_t
{
  int            value;
  struct node_t* next; 
} Node;

int removeValue(Node** ppHead, int value);

#endif //LINKED_LIST_PRACTICE
