#include "linked_list.h"

#include <assert.h>
#include <stdlib.h>
#include <stdio.h>

Node* createNode(int value) {
  Node* node  = (Node*)malloc(sizeof(Node));
  node->value = value;
  node->next  = 0; /* NULL */
  return node;
}

Node* createList(int size) {
  Node* last_node = createNode(0);
  Node* head = last_node;
  for (int i = 1; i < size; ++i) {
    Node *node = createNode(0);
    last_node->next = node;
    last_node = node;
  }
  return head;
}

void freeList(Node *pHead) {
  while (pHead) {
    Node *pNext = pHead->next;
    free(pHead);
    pHead = pNext;
  }
}

int listSize(Node *pHead) {
  int count = 0;
  while (pHead) {
    ++count;
    pHead = pHead->next;
  }
  return count;
}

/* NULL list, we don't find the value */
int testEmptyList() {
  Node *null_node = 0;

  int rv = removeValue(&null_node, 1);

  assert(rv == 0);
  assert(null_node == 0);

  freeList(null_node);

  return 1;
}

/* First value of the list removed, we now have an empty (null) list */
int testFirstValue() {
  Node *first_node = createNode(5);

  Node** ppHead = &first_node;
  int rv = removeValue(ppHead, first_node->value);

  assert(rv == 1);
  assert(*ppHead == 0);
  assert(listSize(first_node) == 0);

  freeList(first_node);

  return 1;
}

/* Remove function returns 0 if it can't find the value */
int testMissingValue() {
  Node *head = createList(6);

  Node **ppHead = &head;
  assert(listSize(head) == 6);

  int rv = removeValue(ppHead, 5);
  
  assert(rv == 0); 
  assert(listSize(head) == 6);

  freeList(*ppHead);

  return 1;
}

/* Last list value correctly removed */
int testLastValue() {
  Node *head = createList(6);
  /* Set last value to 5 */
  Node *node = head;
  while (node->next) {
    node = node->next;
  }
  node->value = 5;

  Node **ppHead = &head;
  assert(listSize(head) == 6);

  int rv = removeValue(ppHead, 5);

  assert(rv == 1);
  assert(*ppHead == head);
  assert(listSize(head) == 5);

  freeList(*ppHead);

  return 1;
}

/* Remove some value in the middle */
int testMiddleValue() {
  int LIST_SIZE = 6;
  Node *head = createList(LIST_SIZE);
  Node **ppHead = &head;

  /* Set some value in the middle */
  Node *node = head;
  int idx = LIST_SIZE / 2;
  int i = 0;
  while (i++ < idx) {
    node = node->next;
  }
  node->value = 5;

  assert(listSize(head) == LIST_SIZE);

  int rv = removeValue(ppHead, 5);

  assert(rv == 1);
  assert(*ppHead == head);
  assert(listSize(head) == LIST_SIZE-1);

  freeList(*ppHead);

  return 1;
}

int main(int argc, char **argv) {

  int rv = 1;
  
  rv = rv && testEmptyList();

  rv = rv && testFirstValue();

  rv = rv && testMissingValue();

  rv = rv && testLastValue(); 

  rv = rv && testMiddleValue(); 

  if (rv) {
    printf("All tests passed\n");
    return 0;
  }

  return 1;
}
