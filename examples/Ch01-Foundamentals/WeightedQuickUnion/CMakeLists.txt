CMAKE_MINIMUM_REQUIRED(VERSION 2.8)
PROJECT(WeightedQuickUnion)
SET(ROOT ../../..)
INCLUDE_DIRECTORIES(${ROOT}/include)
SET(SOURCES
	main.c
	${ROOT}/src/DisjointSet.c
	${ROOT}/src/IntegerSequence.c
)
ADD_EXECUTABLE(union ${SOURCES})
