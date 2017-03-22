CMAKE_MINIMUM_REQUIRED(VERSION 2.8)
PROJECT(IntrusiveLinkedList)
SET(ROOT ../../..)
INCLUDE_DIRECTORIES(${ROOT}/include)
SET(SOURCES
	main.c
	${ROOT}/src/IntrusiveLinkedList.c
)
ADD_EXECUTABLE(list ${SOURCES})
