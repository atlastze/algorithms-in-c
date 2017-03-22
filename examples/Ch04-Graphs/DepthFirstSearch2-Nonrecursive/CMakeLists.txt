CMAKE_MINIMUM_REQUIRED(VERSION 2.8)
PROJECT(DepthFirstSearch2)
SET(ROOT ../../..)
INCLUDE_DIRECTORIES(${ROOT}/include)
SET(SOURCES
	main.c
	${ROOT}/src/IntrusiveLinkedList.c
	${ROOT}/src/IntegerSequence.c
	${ROOT}/src/DisjointSet.c
	${ROOT}/src/Graph.c
	${ROOT}/src/EdgeListGraph.c
)
ADD_EXECUTABLE(dfsearch2 ${SOURCES})
