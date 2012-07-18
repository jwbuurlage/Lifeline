#-- Cmake configuration
cmake_minimum_required( VERSION 2.8 )
project (Lifeline)

set(CMAKE_C_FLAGS "-std=c99")

#-- Then build each samples respectively
add_subdirectory(lib)
add_subdirectory(apps/Tests)
