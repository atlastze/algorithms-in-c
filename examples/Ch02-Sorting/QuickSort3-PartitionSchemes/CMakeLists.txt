CMAKE_MINIMUM_REQUIRED(VERSION 2.8)
PROJECT(QuickSort3)
SET(ROOT ../../..)
INCLUDE_DIRECTORIES(${ROOT}//include)
SET(SOURCES
	    main.c
	    ${ROOT}/src/StopWatch.c
	    ${ROOT}/src/IntegerSequence.c
)
ADD_EXECUTABLE(quicksort3 ${SOURCES})
