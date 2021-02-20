#!/usr/bin/python3

"""Sparse Similarity: Cracking the Coding Interview 6, 17.26

Given a long list of input documents, return similarity for pairs of documents.
Similarity is assumed to be 0 for most pairs.

Similarity is defined as size(intersection) / size(union) for pairs of docs.
"""

from typing import Sequence
import unittest


def _union_size(doc1: Sequence[int], doc2: Sequence[int]) -> int:
    """Computes union size of doc1, doc2."""

    # TODO - This could be optimized if all input docs were sorted
    # Best case is O(n + m) time.
    s = set(doc1)
    s.update(doc2)

    return len(s)


def compute_doc_similarity(documents: {int: Sequence[int]}) -> {(int, int), float}:
    # Find all documents with characters in common: O(D) using HashMap
    char_to_doc_id = {}
    for docid, data in documents.items():
        for char in data:
            char_to_doc_id.setdefault(char, []).append(docid)

    # Find intersection of all documents:
    # O(c * d**2) for c = chars, d = docs in result
    doc_pair_to_count = {}
    for char, docids in char_to_doc_id.items():
        docs = sorted(docids)
        for i, docid in enumerate(docs):
            for j in range(i + 1, len(docs)):
                key = (docid, docs[j])  # Key always has lower docid first
                doc_pair_to_count[key] = doc_pair_to_count.get(key, 0) + 1

    # Iterate and compute result: O(m *
    result = {}
    for doc_pair, count in doc_pair_to_count.items():
        result[doc_pair] = count / _union_size(
            documents[doc_pair[0]],
            documents[doc_pair[1]])

    return result

DOCUMENTS = {
    13: [14, 15, 100, 9, 3],
    16: [32, 1, 9, 3, 5],
    19: [15, 29, 2, 6, 8, 7],
    24: [7, 10],
}

EXPECTED_RESULT = {
    (13, 19): 0.1,
    (13, 16): 0.25,
    (19, 24): 0.142857
}


class SparseSimilarityTest(unittest.TestCase):

    def test_book_example(self):
        result = compute_doc_similarity(DOCUMENTS)

        self.assertEqual(len(result), len(EXPECTED_RESULT))
        for key, value in result.items():
            self.assertIn(key, EXPECTED_RESULT)
            self.assertTrue(abs(value - EXPECTED_RESULT[key]) < 1.0e-4)

if __name__ == '__main__':
    unittest.main()
