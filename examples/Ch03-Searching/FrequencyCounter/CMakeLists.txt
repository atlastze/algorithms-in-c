CMAKE_MINIMUM_REQUIRED(VERSION 2.8)
PROJECT(FrequencyCounter)

FIND_PACKAGE(FLEX)
FLEX_TARGET(Scanner scanner.l  ${CMAKE_CURRENT_SOURCE_DIR}/scanner.c)

#MESSAGE("---------------------------------")
#MESSAGE(${CMAKE_CURRENT_SOURCE_DIR})
#MESSAGE(${FLEX_Scanner_OUTPUTS})
#MESSAGE("---------------------------------")

SET(ROOT ../../..)
INCLUDE_DIRECTORIES(${ROOT}/include)
AUX_SOURCE_DIRECTORY(. SOURCES)
ADD_EXECUTABLE(frequency
	${FLEX_Scanner_OUTPUTS}
	${SOURCES}
	${ROOT}/src/HashCode.c
	${ROOT}/src/IntrusiveLinkedList.c
	${ROOT}/src/SymbolTable.c
)
