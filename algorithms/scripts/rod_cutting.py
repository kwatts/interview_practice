#!/usr/bin/env python3

"""Rod cutting problem, with memoization."""

from typing import Sequence

# Prices for a rod of size i (for rod sizes [0,10]).
#.        0. 1  2. 3. 4. 5.  6.  7.  8.  9,  10
PRICES = [0, 1, 5, 8, 9, 10, 17, 17, 20, 24, 30]

# Cache results from find_best_cut
FIND_BEST_CUT_MEMO = {}


# Returns optimal cut strategy, in lengths, for a rod of input size k
def find_best_cut(k : int) -> Sequence[int]:
    if k == 0:
        return []
    
    if k in FIND_BEST_CUT_MEMO:
        return FIND_BEST_CUT_MEMO[k]
    
    best_value = -1
    best_cut = []
    
    # Start with full length log
    if k < len(PRICES):
        best_value = PRICES[k]
        best_cut = []
    
    for first_cut in range(1, min(len(PRICES), k)):
        remaining_len = k - first_cut
        smaller_cut = find_best_cut(remaining_len)
        curr_value = PRICES[first_cut] + _cut_value(remaining_len, smaller_cut)
        
        if curr_value > best_value:
            best_value = curr_value
            best_cut = [ first_cut ]
            best_cut.extend(smaller_cut)
            
    FIND_BEST_CUT_MEMO[k] = best_cut
    return best_cut

def _cut_value(k : int, cut_lengths : Sequence[int]) -> int:
    """Return the value of a cut rod of input size "k", with the given cut sequence."""
    result = 0
    total_len = 0
    for cut in cut_lengths:
        result += PRICES[cut]
        total_len += cut
    
    remainder = k - total_len
    if remainder > 0:
        result += PRICES[remainder]

    assert remainder >= 0, f'Expected length: {k}, computed {total_len}'
    return result

def print_cut_rod(k):
    cut_lengths = find_best_cut(k)
    
    print(f'Rod length: {k}, value: ' + str(_cut_value(k, cut_lengths)))
    print('Cuts lengths: [' + ', '.join([str(c) for c in cut_lengths]) + ']')
    
    line = ''
    total_len = 0
    for cut in cut_lengths:
        total_len += cut
        line += '. ' * (cut - 1)
        line += '.'
        line += '|'
    line += '. ' * (k - total_len) # remainder
        
    print(line)
    print()


print_cut_rod(1)
print_cut_rod(2)
print_cut_rod(7)
print_cut_rod(10)
print_cut_rod(11)
print_cut_rod(25)
