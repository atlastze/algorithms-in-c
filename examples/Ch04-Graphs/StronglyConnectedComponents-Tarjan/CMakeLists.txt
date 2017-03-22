CMAKE_MINIMUM_REQUIRED(VERSION 2.8)
PROJECT(StronglyConnectedComponents2)
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
ADD_EXECUTABLE(scc2 ${SOURCES})
