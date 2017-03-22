############################################################
# Project file
# Makefile will include this project file
############################################################

# Specify target name
TARGET = prim2

# Specify the #include directories which should be searched when compiling the project.
INCLUDEPATH = . $(ROOT)/include
# Specify the source directories which should be searched when compiling the project.
DEPENDPATH = . $(ROOT)/src

# Defines the header files for the project.
HEADERS = Vector.h IntrusiveLinkedList.h Graph.h \
IntegerSequence.h IndexPriorityQueue.h EdgeListGraph.h DisjointSet.h

# Defines the source files for the project.
SOURCES = main.c IntrusiveLinkedList.c Graph.c \
IntegerSequence.c EdgeListGraph.c DisjointSet.c
