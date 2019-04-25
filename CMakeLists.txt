cmake_minimum_required (VERSION 2.6)

add_subdirectory(./ccsh ccsh EXCLUDE_FROM_ALL)

include_directories(./ccsh/include)
include_directories(./ccsh/wrappers)

include_directories("include")

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11") # ensure C++11
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -pedantic -Wall -Wextra")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wrestrict -Wlogical-op -Wduplicated-cond -Wduplicated-branches -Wnull-dereference -Wold-style-cast -Wnon-virtual-dtor -Woverloaded-virtual")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g")

add_subdirectory(lib)

add_subdirectory(dirs)
add_subdirectory(program)

