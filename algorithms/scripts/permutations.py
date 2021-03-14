#!/usr/bin/env python3

from typing import Sequence

# Print permutations of a dataset, in order. 

# Pass "prefix" to allow printing in order without buffering all permutations.

def permutations(dataset : Sequence[str], k, prefix = None):
    """Print permutations of all selements in 'dataset' (with duplicates)."""
    if prefix is None:
        prefix = ''
    
    if not dataset or k == 0:
        print(prefix)
        return
    
    for c in dataset:
        permutations(dataset, k-1, prefix + c)

def permutations_no_dups(dataset : Sequence[str], k, prefix = None):
    """Print permutations of all selements in 'dataset' (with duplicates)."""
    if prefix is None:
        prefix = ''
    
    if not dataset or k == 0:
        print(prefix)
        return
    
    for c in dataset:
        next_dataset = dataset[:]
        next_dataset.remove(c)
        
        permutations_no_dups(next_dataset, k-1, prefix + c)

        
print('k = 2')
permutations(['a','b','c'], 2)
print('k = 2 (no dups)')
permutations_no_dups(['a','b','c'], 2)


print('k = 3')
permutations(['a','b','c'], 3)
print('k = 3 (no dups)')
permutations_no_dups(['a','b','c'], 3)
