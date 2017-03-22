CMAKE_MINIMUM_REQUIRED(VERSION 2.8)
PROJECT(PolynomialAddition)
SET(ROOT ../../..)
INCLUDE_DIRECTORIES(. ${ROOT}/include)
SET(SOURCES
	main.c
	Polynomial.c
	${ROOT}/src/SinglyLinkedList.c
)
ADD_EXECUTABLE(polyn ${SOURCES})
