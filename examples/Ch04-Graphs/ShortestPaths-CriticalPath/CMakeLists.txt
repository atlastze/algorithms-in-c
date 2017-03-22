CMAKE_MINIMUM_REQUIRED(VERSION 2.8)
PROJECT(CriticalPath)
SET(ROOT ../../..)
INCLUDE_DIRECTORIES(${ROOT}/include)
SET(SOURCES
	main.c
	${ROOT}/src/IntrusiveLinkedList.c
	${ROOT}/src/Graph.c
	${ROOT}/src/IntegerSequence.c
	${ROOT}/src/DisjointSet.c
	${ROOT}/src/EdgeListGraph.c
)
ADD_EXECUTABLE(kpath ${SOURCES})
