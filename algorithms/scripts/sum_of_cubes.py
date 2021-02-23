#!/usr/bin/python3

"""Print all integer solutions of the equation:

a**3 + b**3 = c**3 + d**3

For input values a,b,c,d from [0, 1000).

 - Only print a single permutation for each solution
 - Non-trivial solutions only: (a,c) != (b,d)
"""


def print_solution(header, a, b, c, d):
    print(f'{header}: {a}**3 + {b}**3 == {c}**3 + {d}**3')


def print_sum_of_cubes_naive(max_value):
    """Naive solution in O(n**4) time."""
    for a in range(0, max_value):
        for b in range(a, max_value):
            for c in range(0, max_value):
                for d in range(c, max_value):
                    if a == c and b == d:
                        continue

                    if a**3 + b**3 == c**3 + d**3:
                        print_solution('Naive', a, b, c, d)


def print_sum_of_cubes(max_value):
    """Optimized version in O(n**2) time."""
    cache = {}  # { sum : [(a, b)] }

    for a in range(0, max_value):
        for b in range(a, max_value):
            cache.setdefault(a**3 + b**3, []).append((a, b))

    for pairs in cache.values():
        for pair1 in pairs:
            for pair2 in pairs:
                if pair1 == pair2:
                    continue
                print_solution('Opt', pair1[0], pair1[1], pair2[0], pair2[1])


if __name__ == '__main__':
    # print_sum_of_cubes_naive(100)

    print_sum_of_cubes(100)
