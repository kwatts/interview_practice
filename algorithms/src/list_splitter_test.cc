#include <gtest/gtest.h>

#include <fstream>
#include <iostream>
#include <ostream>
#include <string>
#include <vector>

using namespace std;

class SinglyLinkedListNode {
 public:
  int data;
  SinglyLinkedListNode* next;

  SinglyLinkedListNode(int node_data) {
    this->data = node_data;
    this->next = nullptr;
  }
};

class SinglyLinkedList {
 public:
  SinglyLinkedListNode* head;
  SinglyLinkedListNode* tail;

  SinglyLinkedList() {
    this->head = nullptr;
    this->tail = nullptr;
  }

  void insert_node(int node_data) {
    SinglyLinkedListNode* node = new SinglyLinkedListNode(node_data);

    if (!this->head) {
      this->head = node;
    } else {
      this->tail->next = node;
    }

    this->tail = node;
  }
};

void print_singly_linked_list(SinglyLinkedListNode* node, string sep,
                              ofstream& fout) {
  while (node) {
    fout << node->data;

    node = node->next;

    if (node) {
      fout << sep;
    }
  }
}

/*
 * Complete the 'createLinkedList' function below.
 *
 * The function is expected to return an INTEGER_SINGLY_LINKED_LIST.
 * The function accepts INTEGER_SINGLY_LINKED_LIST head as parameter.
 */

/*
 * For your reference:
 *
 * SinglyLinkedListNode {
 *     int data;
 *     SinglyLinkedListNode* next;
 * };
 *
 */

/**
 * Splits list into odd/even pairs
 *
 * @param p_head Head of initial list
 * @param pp_tail Tail of odd list, used for list concatenation
 * @param pp_head_even Head of even list
 */
void splitLinkedList(SinglyLinkedListNode* p_head,
                     SinglyLinkedListNode** pp_tail,
                     SinglyLinkedListNode** pp_head_even) {
  if (!p_head) {
    *pp_head_even = 0x0;
    return;
  }

  // odds - p_head
  // evens - *pp_head_even
  *pp_head_even = p_head->next;

  SinglyLinkedListNode* p_odd = p_head;
  SinglyLinkedListNode* p_even = *pp_head_even;

  SinglyLinkedListNode* p_even_last = nullptr;
  while (p_head) {
    p_odd = p_head;
    p_even = p_head->next;

    p_odd->next = p_even ? p_even->next : 0x0;
    // !!!! THIS WAS ORIGINAL BUG IN THE CODE, "next" NOT ASSIGNED FOR EVENS !!!
    //        if (p_even_last && p_even) {
    if (p_even_last) {
      p_even_last->next = p_even;
    }
    p_even_last = p_even;

    // Update p_head to skip next 2 values
    if (!p_even) {
      break;
    }
    p_head = p_even->next;
  }

  // Need to update p_even->next when it breaks out.
  if (p_even) {
    p_even->next = 0x0;
  }
  p_odd->next = 0x0;

  *pp_tail = p_odd;
}

SinglyLinkedListNode* createLinkedList(SinglyLinkedListNode* head) {
  SinglyLinkedListNode* odds_tail = 0x0;
  SinglyLinkedListNode* evens = 0x0;

  splitLinkedList(head, &odds_tail, &evens);
  while (evens) {
    odds_tail->next = evens;  // concat lists

    SinglyLinkedListNode* evens_tail = 0x0;
    splitLinkedList(evens, &odds_tail, &evens_tail);
    evens = evens_tail;
  }

  return head;
}

// TEST CODE BELOW THIS LINE

// Verifies lists have the correct contents with NULL termination.
void verifyListContents(SinglyLinkedListNode* p_head,
                        vector<int> const& contents) {
  if (contents.empty()) {
    ASSERT_EQ(p_head, nullptr);
    return;
  }

  int i = 0;
  while (p_head) {
    ASSERT_LT(i, contents.size())
        << "Off the end of the list. Tail->next != NULL";

    ASSERT_EQ(p_head->data, contents[i]);
    i++;

    p_head = p_head->next;
  }

  ASSERT_EQ(i, contents.size());
}

TEST(splitLinkedList, splitEmpty) {
  SinglyLinkedListNode* odds_tail = 0x0;
  SinglyLinkedListNode* empty = 0x0;
  splitLinkedList(0x0, &odds_tail, &empty);

  ASSERT_EQ(odds_tail, nullptr);
  ASSERT_EQ(empty, nullptr);
}

TEST(splitLinkedList, splitOne) {
  SinglyLinkedList list;
  list.insert_node(1);

  SinglyLinkedListNode* list_tail = 0x0;
  SinglyLinkedListNode* empty = 0x0;
  splitLinkedList(list.head, &list_tail, &empty);

  verifyListContents(list.head, {1});
  ASSERT_EQ(list_tail, list.head);
  ASSERT_EQ(empty, nullptr);
}

TEST(splitLinkedList, splitTwo) {
  SinglyLinkedList list;
  list.insert_node(1);
  list.insert_node(2);

  SinglyLinkedListNode* list_tail = 0x0;
  SinglyLinkedListNode* evens = 0x0;
  splitLinkedList(list.head, &list_tail, &evens);

  verifyListContents(list.head, {1});
  verifyListContents(evens, {2});
  ASSERT_EQ(list_tail, list.head);
}

TEST(splitLinkedList, splitThree) {
  SinglyLinkedList list;
  list.insert_node(1);
  list.insert_node(2);
  list.insert_node(3);

  SinglyLinkedListNode* list_tail = 0x0;
  SinglyLinkedListNode* evens = 0x0;
  splitLinkedList(list.head, &list_tail, &evens);

  verifyListContents(list.head, {1, 3});
  verifyListContents(evens, {2});
  ASSERT_EQ(list_tail->data, 3);
}

TEST(splitLinkedList, splitMany) {
  SinglyLinkedList list;
  for (int i = 1; i < 10; ++i) {
    list.insert_node(i);
  }

  SinglyLinkedListNode* list_tail = 0x0;
  SinglyLinkedListNode* evens = 0x0;
  splitLinkedList(list.head, &list_tail, &evens);

  verifyListContents(list.head, {1, 3, 5, 7, 9});
  verifyListContents(evens, {2, 4, 6, 8});
  ASSERT_EQ(list_tail->data, 9);
}

TEST(createLinkedList, twoItems) {
  SinglyLinkedList list;
  list.insert_node(1);
  list.insert_node(2);

  SinglyLinkedListNode* result = createLinkedList(list.head);

  verifyListContents(result, {1, 2});
}

TEST(createLinkedList, threeItems) {
  SinglyLinkedList list;
  list.insert_node(1);
  list.insert_node(2);
  list.insert_node(3);

  SinglyLinkedListNode* result = createLinkedList(list.head);

  verifyListContents(result, {1, 3, 2});
}

TEST(createLinkedList, manyItems) {
  SinglyLinkedList list;
  list.insert_node(1);
  list.insert_node(12);
  list.insert_node(3);
  list.insert_node(14);
  list.insert_node(5);
  list.insert_node(16);

  SinglyLinkedListNode* result = createLinkedList(list.head);

  verifyListContents(result, {1, 3, 5, 12, 16, 14});
}
