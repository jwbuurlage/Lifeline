#-- Cmake configuration
cmake_minimum_required( VERSION 2.8 )
project (Lifeline)

#Add the LDEBUG preprocessor definition
ADD_DEFINITIONS( -DLDEBUG )

LINK_DIRECTORIES(${CMAKE_BINARY_DIR}/../bin)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/../bin)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/../bin)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/../bin)
set(CMAKE_C_FLAGS "-std=c99")

#-- Then build each samples respectively
add_subdirectory(../lib/build ../lib/build)
add_subdirectory(../apps/Tests/build ../apps/Tests/build)
