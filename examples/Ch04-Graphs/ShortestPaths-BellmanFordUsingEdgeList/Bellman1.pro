############################################################
# Project file
# Makefile will include this project file
############################################################

# Specify target name
TARGET = bellman1

# Specify the #include directories which should be searched when compiling the project.
INCLUDEPATH = . $(ROOT)/include
# Specify the source directories which should be searched when compiling the project.
DEPENDPATH = . $(ROOT)/src

# Defines the header files for the project.
HEADERS = Vector.h EdgeListGraph.h DisjointSet.h IntegerSequence.h

# Defines the source files for the project.
SOURCES = main.c EdgeListGraph.c DisjointSet.c IntegerSequence.c
