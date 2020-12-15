#include "search_tree.h"

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

// Verify that invariants are maintained (l < val < r).
int verifyTreeValid(Node *pHead) {
  if (!pHead) {
    return 1;
  }
  if (pHead->left && pHead->left->value > pHead->value) {
    return 0;
  }
  if (pHead->right && pHead->right->value < pHead->value) {
    return 0;
  }
  // TODO(kevinwatts) Iteratively verify tree, no recursion.
  return verifyTreeValid(pHead->left) && verifyTreeValid(pHead->right);
}

Node *createNode(int value) {
  Node *node = (Node *)malloc(sizeof(Node));
  node->value = value;
  node->left = 0; /* NULL */
  node->right = 0;
  return node;
}

#define LIKELY(x) __builtin_expect(!!(x), 1)
#define UNLIKELY(x) __builtin_expect(!!(x), 0)

#define CHECK(_val, _msg)                                                      \
  do {                                                                         \
    if (UNLIKELY(!_val)) {                                                     \
      fprintf(stderr, "Assertion failed!\n");                                  \
      fprintf(stderr, "Expression: " #_val "\n");                              \
      fprintf(stderr, "File: %s, line: %d\n", __FILE__, __LINE__);             \
      fprintf(stderr, "Message: %s\n", _msg);                                  \
      exit(1);                                                                 \
    }                                                                          \
  } while (0)

int testValidSimple() {
  Node *pHead = createNode(0);
  pHead->left = createNode(-1);
  pHead->right = createNode(1);

  CHECK(verifyTreeValid(pHead), "Invalid tree!");

  Node *tmp = pHead->right;
  pHead->right = pHead->left;
  pHead->left = tmp;
  CHECK(!verifyTreeValid(pHead), "Valid tree!");

  freeTree(&pHead);
  CHECK(!pHead, "Tree not properly freed!");

  return 1;
}

int testContainsEmpty() {
  Node *pHead = 0;

  CHECK(verifyTreeValid(pHead), "Invalid tree!");
  CHECK(!containsValue(pHead, 5), "Empty tree contains no values!");

  freeTree(&pHead);
  CHECK(!pHead, "Tree not properly freed!");

  return 1;
}

int testContainsSimple() {
  const int kVal = 5;

  Node *pHead = createNode(kVal);

  CHECK(verifyTreeValid(pHead), "Invalid tree!");
  CHECK(containsValue(pHead, kVal), "Doesn't contain value!");
  CHECK(!containsValue(pHead, 3), "Contains invalid value!");

  freeTree(&pHead);
  CHECK(!pHead, "Tree not properly freed!");

  return 1;
}

int testContainsChildren() {
  const int kRootVal = 5;
  const int kChildVal = 3;
  const int kGChildVal = 4;

  Node *pHead = createNode(kRootVal);
  pHead->left = createNode(kChildVal);
  pHead->left->right = createNode(kGChildVal);

  CHECK(verifyTreeValid(pHead), "Invalid tree!");
  CHECK(containsValue(pHead, kRootVal), "Doesn't contain root value!");
  CHECK(containsValue(pHead, kChildVal), "Doesn't contain child value!");
  CHECK(containsValue(pHead, kGChildVal), "Doesn't contain grandchild value!");
  CHECK(!containsValue(pHead, 7), "Contains bogus value!");

  freeTree(&pHead);
  CHECK(!pHead, "Tree not properly freed!");

  return 1;
}

int testRemoveEmpty() {
  Node *pHead = 0;

  CHECK(verifyTreeValid(pHead), "Invalid tree!");
  CHECK(!removeValue(&pHead, 1), "Able to remove from empty tree!");
  CHECK(verifyTreeValid(pHead), "Invalid tree after remove!");

  freeTree(&pHead);
  CHECK(!pHead, "Tree not properly freed!");

  return 1;
}

int testRemoveHeadOnly() {
  const int kValue = 5;

  Node *pHead = createNode(kValue);

  CHECK(verifyTreeValid(pHead), "Invalid tree!");
  CHECK(!removeValue(&pHead, 1), "Able to remove invalid value!");
  CHECK(removeValue(&pHead, kValue), "Unable to remove head value!");
  CHECK(verifyTreeValid(pHead), "Invalid tree after remove!");

  freeTree(&pHead);
  CHECK(!pHead, "Tree not properly freed!");

  return 1;
}

int testRemoveSingleChild() {
  const int kValue = 5;

  Node *pHead = createNode(kValue);
  pHead->left = createNode(kValue - 1);

  CHECK(verifyTreeValid(pHead), "Invalid tree!");
  CHECK(!removeValue(&pHead, 1), "Able to remove invalid value!");
  CHECK(removeValue(&pHead, kValue),
        "Unable to remove head value with single child!");
  CHECK(verifyTreeValid(pHead), "Invalid tree after remove!");
  CHECK(!containsValue(pHead, kValue), "Unexpected value present!");
  CHECK(containsValue(pHead, kValue - 1), "Expected value missing!");

  freeTree(&pHead);
  CHECK(!pHead, "Tree not properly freed!");

  return 1;
}

int testRemoveDoubleChild() {
  const int kValue = 5;

  Node *pHead = createNode(kValue);
  pHead->left = createNode(kValue - 1);
  pHead->right = createNode(kValue + 1);

  CHECK(verifyTreeValid(pHead), "Invalid tree!");
  CHECK(!removeValue(&pHead, 1), "Able to remove invalid value!");
  CHECK(removeValue(&pHead, kValue),
        "Unable to remove head value with double child!");
  CHECK(verifyTreeValid(pHead), "Invalid tree after remove!");
  CHECK(!containsValue(pHead, kValue), "Unexpected value present!");
  CHECK(containsValue(pHead, kValue - 1), "Expected value missing!");

  freeTree(&pHead);
  CHECK(!pHead, "Tree not properly freed!");

  return 1;
}

int main(int argc, char **argv) {
  int result = 1;

  result = testValidSimple() && result;

  // Contains
  result = testContainsEmpty() && result;
  result = testContainsSimple() && result;
  result = testContainsChildren() && result;

  result = testRemoveEmpty() && result;
  result = testRemoveHeadOnly() && result;
  result = testRemoveSingleChild() && result;
  result = testRemoveDoubleChild() && result;

  if (result) {
    fprintf(stdout, "All tests passed!\n");
    return 0;
  }

  return 1;
}
