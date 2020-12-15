#!/usr/bin/python

def incr_decimal(num):
    """Increment a decimal coded number.

    [1,2,3] -> [1,2,3] # 123 -> 124.
    """
    for idx in range(len(num) - 1, -1, -1):
        incremented = num[idx] + 1
        num[idx] = incremented % 10
        if incremented < 10:
            return num
    return [1] + [0] * len(num)

def encode_decimal_list(num):
    """Encode a number as a decimal list"""
    if num == 0:
        return [ 0 ]

    rv = []
    while num:
        rv.insert(0, num % 10)
        num = num / 10
    return rv

def decode_decimal_list(lst):
    """Decode a decimal list into a number"""
    num = 0
    for i, l in enumerate(reversed(lst)):
        num += l * 10**i
    return num

def test_encode():
    assert [1, 0, 0] == encode_decimal_list(100)
    assert [1, 2, 3] == encode_decimal_list(123)
    assert [0] == encode_decimal_list(0)
    assert [9, 5] == encode_decimal_list(95)

def test_decode():
    assert 100 == decode_decimal_list([1,0,0])
    assert 123 == decode_decimal_list([1,2,3])
    assert 99 == decode_decimal_list([9,9])

def test_increment():
    assert [1,2,4] == incr_decimal([1,2,3])
    assert [1,0,0] == incr_decimal([9,9])
    assert [6] == incr_decimal([5])
    assert [1] == incr_decimal([0])
    assert [1,2,0] == incr_decimal([1,1,9])
    assert [2,0,0] == incr_decimal([1,9,9])

test_encode()
test_decode()
test_increment()
