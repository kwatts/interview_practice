#include "search_tree.h"

#include "assert.h"
#include "stdlib.h"

int containsValue(Node *pHead, int value) {
  while (pHead) {
    if (value == pHead->value) {
      return 1;
    }

    if (value < pHead->value) {
      pHead = pHead->left;
    } else {
      pHead = pHead->right;
    }
  }

  return 0;
}

void freeTree(Node **ppHead) {
  if (!*ppHead) {
    return;
  }

  // Clear the children.
  freeTree(&((*ppHead)->left));
  freeTree(&((*ppHead)->right));

  free(*ppHead);
  *ppHead = 0;
}

int removeValue(Node **ppHead, int value) {
  if (!(*ppHead)) {
    return 0;
  }

  while (*ppHead) {
    if (value < (*ppHead)->value) {
      ppHead = &(*ppHead)->left;
      continue;
    } else if (value > (*ppHead)->value) {
      ppHead = &(*ppHead)->right;
      continue;
    }

    assert(value == (*ppHead)->value);
    // No children present, just remove node.
    if (!(*ppHead)->left && !(*ppHead)->right) {
      free(*ppHead);
      *ppHead = 0;
      return 1;
    } else if ((*ppHead)->left && (*ppHead)->right) {
      // Both children present. Find left-most child of right node, move to
      // parent.
      Node **ppLeftest = &(*ppHead)->right;
      while ((*ppLeftest)->left) {
        ppLeftest = &(*ppLeftest)->left;
      }

      // Move leftest child into *ppHead's spot.
      Node *old_head = *ppHead;
      Node *leftest_child = (*ppLeftest)->right;

      *ppHead = *ppLeftest;
      (*ppHead)->left = old_head->left;
      (*ppHead)->right = old_head->right;

      // leftest's remaining child into parent's left spot.
      (*ppLeftest) = leftest_child;

      free(old_head);

      return 1;
    } else {
      // One child of node to be deleted. Move child node into parent's place.
      Node *which_child = (*ppHead)->left ? (*ppHead)->left : (*ppHead)->right;
      Node *old_head = *ppHead;
      *ppHead = which_child;
      free(old_head);
      return 1;
    }
  }

  return 0;
}
