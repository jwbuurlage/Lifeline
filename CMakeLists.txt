#-- Cmake configuration
cmake_minimum_required( VERSION 2.8 )
project (Lifeline)

LINK_DIRECTORIES(${CMAKE_BINARY_DIR}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_C_FLAGS "-std=c99")

#-- Then build each samples respectively
add_subdirectory(lib)
add_subdirectory(apps/Tests)
