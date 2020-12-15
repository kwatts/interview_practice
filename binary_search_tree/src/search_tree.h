#ifndef BINARY_SEARCH_TREE_PRACTICE
#define BINARY_SEARCH_TREE_PRACTICE

// Binary search tree.
// left < value < right
typedef struct node_t {
  int value;
  struct node_t *left;
  struct node_t *right;
} Node;

// Return 1 if tree contains value.
int containsValue(Node *pHead, int value);

// Free the tree and set pHead to 0.
void freeTree(Node **ppHead);

// Remove first instance of value from tree.
// Return 1 if value removed.
int removeValue(Node **ppHead, int value);

#endif // BINARY_SEARCH_TREE_PRACTICE
