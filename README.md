# Data Structures and Algorithms in C

An implementation of data structures and algorithms in C, including intrusive linked list, generic vector, hash table, index priority queue, adjacency list representation of graph, and so on.

## Introduction

This public repository implement C source code for the algorithms and clients in the textbook Algorithms, 4th/e by Robert Sedgewick and Kevin Wayne.
* Chapter 1: Fundamentals includes singly/doubly linked list, vector and intrusive linked list data structures and applications.
* Chapter 2: Sorting includes quicksort and also features a binary heap implementation of a priority queue.
* Chapter 3: Searching includes classic symbol-table implementations, binary search trees and a word counter implemented by flex.
* Chapter 4: Graphs includes the most important graph-processing problems, including depth-first search, breadth-first search, minimum spanning trees, and shortest paths.

## Building examples
### Requirements
* C Compiler(clang, gcc or Visual Studio 2015)
* flex 2.5

### Building  
Use GNU Make:

    make

Alternatively, use CMake:

    mkdir build
    cd build
    cmake ..
    make

On Windows(Visual Studio 2015 + CMake):

    mkdir build
    cd build
    cmake .. -G "NMake Makefiles")
    nmake

## References
* Robert Sedgewick and Kevin Wayne. Algorithms, 4th/e
* Thomas H. Cormen et al. Introduction to Algorithms, 3rd/e
* Michael T. Goodrich et al. Data Structures and Algorithms in C++, 2nd/e
* https://activities.tjhsst.edu/sct/lectures.php
