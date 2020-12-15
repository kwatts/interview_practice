#!/usr/bin/env python

import itertools

import numpy as np
import random

import sys

class BinarySearchTree(object):
    class _BinaryTreeNode(object):
        __slots__ = ['value', 'left', 'right']
        def __init__(self, val):
            self.value = val
            self.left  = None
            self.right = None
    
    def __init__(self, values, key = None):
        self._key_fn = key if key else lambda x: x

        self.set_values(values)
            
    def set_values(self, values):
        self._root = self._BinaryTreeNode(values[0])
        v_iter = iter(values); v_iter.next() # skip first
        for v in v_iter:
            self.insert(v)
        
    def is_empty(self):
        return self._root is None
    
    def root(self):
        return self._root.value if self._root else None
        
    @staticmethod
    def _min_max_height(node):
        l_mn, l_mx = 0, 0
        if node.left: 
            l_mn, l_mx = BinarySearchTree._min_max_height(node.left)
        r_mn, r_mx = 0, 0    
        if node.right: 
            r_mn, r_mx = BinarySearchTree._min_max_height(node.right)
            
        return 1+min(l_mn,r_mn), 1+max(l_mx, r_mx)

        
    def min_max_height(self):
        return self._min_max_height(self._root)
    
    def is_balanced(self):
        mn, mx = self.min_max_height()
        return mx - mn <= 1
            
    def insert(self, val):
        if not self._root:
            self._root = self._BinaryTreeNode(val)
            return
        
        node = self._root
        while node:
            if self._key_fn(val) < self._key_fn(node.value):
                if not node.left:
                    node.left = BinarySearchTree._BinaryTreeNode(val)
                    return
                node = node.left
            else:
                if not node.right:
                    node.right = BinarySearchTree._BinaryTreeNode(val)
                    return
                node = node.right
        
    def _get_node_and_parent(self, val):
        parent = None
        node = self._root
        while node:
            key = self._key_fn(node.value) 
            if key == val:
                return node, parent
            
            parent = node
            if key < val:
                node = node.right
            else:
                node = node.left
                
        return None
        
    def has_value(self, val):
        return self._get_node_and_parent(val) is not None

    # Note
    def remove_value(self, val):
        node_and_parent = self._get_node_and_parent(val)
        if not node_and_parent:
            raise Exception('Unable to remove value, does not exist in tree! ' + str(val))
        node, parent = node_and_parent
        
        # Case where node has no children -> remove parent
        if not node.left and not node.right:
            if not parent:
                self._root = None
                return
             
            if parent.left == node:
                parent.left = None
            else:
                parent.right = None
            return 
        
        # Child only on the right or the left
        def _replace_node(node, replacement):
            node.value = replacement.value
            node.left  = replacement.left
            node.right = replacement.right
        if not node.left:
            _replace_node(node, node.right)
            return
        if not node.right:
            _replace_node(node, node.left)
            return
            
        # Case where node has two children
        # If our right child has no left child -> replace
        if not node.right.left:
            node.value = node.right.value
            node.right = node.right.right
            # node.left unchanged
            return 
        
        # Find "successor" in our right side tree (this is the minimum value)
        successor = node.right
        parent = node
        while successor.left:
            parent = successor
            successor = successor.left
        
        successor_value = successor.value
        
        # Replace successor by own right child
        if successor.right:
            _replace_node(successor, successor.right)
        else:
            parent.left = None # Remove successor 
        
        # Replace node by successor
        node.value = successor_value
            
    @staticmethod
    def _in_order(node):
        if not node:
            return []
        rv = []
        if node.left:
            rv.extend(BinarySearchTree._in_order(node.left))
        rv.append(node.value)
        if node.right:
            rv.extend(BinarySearchTree._in_order(node.right))
        return rv
    
    def in_order(self):
        return self._in_order(self._root)
        
    def is_valid(self):
        if self.is_empty():
            return True
        
        values = self.in_order()
        
        # Python "is_sorted"
        it = iter(values)
        it.next()
        return all(b >= a for a, b in itertools.izip(values, it))
    
# Testing
num_elems = 10
arry = list(np.random.randint(0, 100, num_elems))
search_tree = BinarySearchTree(arry)

values = search_tree.in_order()

print 'In order tree:', values
print 'Valid tree:',search_tree.is_valid()

print 'Has value (first element):', search_tree.has_value(values[0])
print 'Has value (missing element):', search_tree.has_value(values[-1]+1)

print 'Min/max height of tree:', search_tree.min_max_height()
print 'Balanced?:', search_tree.is_balanced()

print 'Removing first node ...', 
search_tree.remove_value(values[0])
print 'In order tree:', search_tree.in_order()
print 'Valid tree:', search_tree.is_valid()

print 'Removing last node ...', 
search_tree.remove_value(values[-1])
print 'In order tree:', search_tree.in_order()
print 'Valid tree:', search_tree.is_valid()

print 'Removing middle node ...', 
search_tree.remove_value(values[len(values) / 2])
print 'In order tree:', search_tree.in_order()
print 'Valid tree:', search_tree.is_valid()

print 'Removing all nodes ...'
while not search_tree.is_empty():
    print
    remaining_vals = search_tree.in_order()
    print '\tRemaining values:', remaining_vals
    to_remove = remaining_vals[random.randint(0, len(remaining_vals)-1)]
    print '\tTo remove:', to_remove
    expected_num_vals = len(remaining_vals) - 1
    search_tree.remove_value(to_remove)
    print '\tValid tree:', search_tree.is_valid()
    actual_num_vals = len(search_tree.in_order())
    if actual_num_vals != expected_num_vals:
        print >> sys.stderr, "Number of values mismatch! Tree was corrupted!"
    