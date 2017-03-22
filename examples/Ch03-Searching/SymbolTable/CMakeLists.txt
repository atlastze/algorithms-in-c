CMAKE_MINIMUM_REQUIRED(VERSION 2.8)
PROJECT(SymbolTable)
SET(ROOT ../../..)
INCLUDE_DIRECTORIES(${ROOT}/include)
SET(SOURCES
	main.c
	${ROOT}/src/IntrusiveLinkedList.c
	${ROOT}/src/HashCode.c
	${ROOT}/src/SymbolTable.c
)
ADD_EXECUTABLE(symtab ${SOURCES})
