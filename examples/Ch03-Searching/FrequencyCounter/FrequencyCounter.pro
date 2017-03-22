############################################################
# Project file
# Makefile will include this project file
############################################################

# Specify target name
TARGET = frequency

# Specify the #include directories which should be searched when compiling the project.
INCLUDEPATH = . $(ROOT)/include

# Specify the source directories which should be searched when compiling the project.
DEPENDPATH = . $(ROOT)/src

# Defines the header files for the project.
HEADERS = HashCode.h SymbolTable.h IntrusiveLinkedList.h

# Defines the source files for the project.
SOURCES = main.c HashCode.c SymbolTable.c IntrusiveLinkedList.c scanner.c
