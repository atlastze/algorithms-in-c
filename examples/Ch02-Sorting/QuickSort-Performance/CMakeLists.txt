CMAKE_MINIMUM_REQUIRED(VERSION 2.8)
PROJECT(Performance)
SET(ROOT ../../..)
INCLUDE_DIRECTORIES(${ROOT}/include)
SET(SOURCES
	main.c
	${ROOT}/src/StopWatch.c
	${ROOT}/src/IntegerSequence.c
)
ADD_EXECUTABLE(ratio ${SOURCES})
