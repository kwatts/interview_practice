#!/usr/bin/python

import bisect
import unittest


class LightBulbsBase(object):
    """Interface class for LightBulbs."""

    def __init__(self, num_bulbs):
        """Initialize N light bulbs to off."""
        self.num_bulbs = num_bulbs

    def is_on(self, index):
        """Return the state of the light bulb at index i."""
        raise NotImplementedError("is_on not implemented for base class!")

    def toggle(self, start, end):
        """Change light bulbs from [start, end). start >= 0, end < N."""
        raise NotImplementedError("toggle not implemented for base class!")


class LightBulbsNaive(LightBulbsBase):
    """Store LightBulb state using boolean list."""

    def __init__(self, num_bulbs):
        super(LightBulbsNaive, self).__init__(num_bulbs)
        self._bulbs = [False for i in range(0, num_bulbs)]

    def is_on(self, index):
        return self._bulbs[index]

    def toggle(self, start, end):
        for i in range(start, end):
            self._bulbs[i] = not self._bulbs[i]


class LightBulbsToggleCount(LightBulbsBase):
    """Store LightBulb state by tracking calls to toggle()."""
    class _Toggle(object):
        ___slots__ = ['start', 'end']

        def __init__(self, start, end):
            self.start = start
            self.end = end

    def __init__(self, num_bulbs):
        super(LightBulbsToggleCount, self).__init__(num_bulbs)
        self._toggles = []

    def is_on(self, index):
        result = False
        for t in self._toggles:
            if index >= t.start and index < t.end:
                result = not result
        return result

    def toggle(self, start, end):
        self._toggles.append(self._Toggle(start, end))


class LightBulbsToggleControlPoints(LightBulbsBase):
    """Store LightBulb state by keep control points, where state change.

    Store points between [0, N). Every point represents a change in the bulb
    state, starting at that index.

    Examples:
    _points = [] # all bulbs off.
    _points = [0] # all bulbs on.
    _points = [ 5, 6 ] # bulb 5 on, only.
    """

    def __init__(self, num_bulbs):
        super(LightBulbsToggleControlPoints, self).__init__(num_bulbs)

        self._points = []

    def is_on(self, index):
        idx = bisect.bisect_right(self._points, index)

        return idx % 2 != 0

    def toggle(self, start, end):
        # If start/end not in self._points, just add them to the list. If they
        # are, remove.
        # TODO(kevinwatts) Use sorted container for O(n logn) insertions.
        # http://grantjenks.com/docs/sortedcontainers/sortedset.html
        start_idx = bisect.bisect_left(self._points, start)
        if start_idx < len(self._points) and self._points[start_idx] == start:
            self._points.remove(start)
        else:
            self._points.insert(start_idx, start)

        end_idx = bisect.bisect_left(self._points, end)
        if end_idx < len(self._points) and self._points[end_idx] == end:
            self._points.remove(end)
        elif end < self.num_bulbs:
            self._points.insert(end_idx, end)


"""Light bulbs test methods.

Test fixtures are designed to be used with a type class, to test the top-level
interface.
class MyBulbTest(object):
    def test_foo(self):
        bulbs_cls = self._class  # Subclass of LightBulbsBase.
        bulbs = self._class(N)

class TestNaive(MyBulbTest, unittest.TestCase):
    _class = LightBulbsNaive
    pass
"""


class SimpleLightBulbTest(object):

    def test_init(self):
        bulbs_cls = self._class  # Subclass of LightBulbsBase.
        print 'Testing class:', bulbs_cls.__name__

        b = bulbs_cls(10)
        for i in range(0, 10):
            self.assertFalse(b.is_on(i), "Bulb %d is on!" % i)

        print 'Toggle [0, 5)'
        b.toggle(0, 5)
        for i in range(0, 5):
            self.assertTrue(b.is_on(i), "Bulb %d is off after toggle!" % i)
        for i in range(5, 10):
            self.assertFalse(b.is_on(i), "Bulb %d is on, no toggle!" % i)

        print 'Toggle [2, 7)'
        b.toggle(2, 7)
        for i in range(0, 2):
            self.assertTrue(b.is_on(i), "Bulb %d is off after toggle!" % i)
        for i in range(2, 5):
            self.assertFalse(b.is_on(i), "Bulb %d is on after 2 toggles!" % i)
        for i in range(5, 7):
            self.assertTrue(b.is_on(i), "Bulb %d is off after toggle!" % i)
        for i in range(7, 10):
            self.assertFalse(b.is_on(i), "Bulb %d is on after no toggles!" % i)

        print 'Toggle [4, 6)'
        b.toggle(4, 6)
        for i in range(0, 2):
            self.assertTrue(b.is_on(i), "Bulb %d is off after toggle!" % i)
        for i in range(2, 4):
            self.assertFalse(b.is_on(i), "Bulb %d is on after 2 toggles!" % i)
        for i in range(4, 5):
            self.assertTrue(b.is_on(i), "Bulb %d is off after toggle!" % i)
        for i in range(5, 6):
            self.assertFalse(b.is_on(i), "Bulb %d is on after no toggles!" % i)
        for i in range(6, 7):
            self.assertTrue(b.is_on(i), "Bulb %d is on after no toggles!" % i)
        for i in range(7, 10):
            self.assertFalse(b.is_on(i), "Bulb %d is on after no toggles!" % i)

        print 'Toggle [8, 10)'
        b.toggle(8, 10)
        for i in range(7, 8):
            self.assertFalse(b.is_on(i), "Bulb %d is on after no toggles!" % i)
        for i in range(8, 10):
            self.assertTrue(b.is_on(i), "Bulb %d is off after toggle!" % i)

        print 'Toggle [0, 10)'
        b.toggle(0, 10)

        print 'Light bulb state:'
        print [b.is_on(i) for i in range(0, 10)]


class TestBulbsNaive(SimpleLightBulbTest, unittest.TestCase):
    _class = LightBulbsNaive
    pass


class TestBulbsToggleCount(SimpleLightBulbTest, unittest.TestCase):
    _class = LightBulbsToggleCount
    pass


class TestBulbsControlPoints(SimpleLightBulbTest, unittest.TestCase):
    _class = LightBulbsToggleControlPoints
    pass

if __name__ == '__main__':
    unittest.main()
