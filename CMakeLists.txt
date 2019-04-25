cmake_minimum_required (VERSION 2.6)

project(ccbs)

add_subdirectory(./ccsh ccsh EXCLUDE_FROM_ALL)

include_directories(./ccsh/include)
include_directories(./ccsh/wrappers)

include_directories("include")

if(WIN32)
    add_definitions(/DUNICODE /D_UNICODE /EHsc /std:c++latest)
    set(CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS ON)
else()
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11") # ensure C++11
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -pedantic -Wall -Wextra")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wrestrict -Wlogical-op -Wduplicated-cond -Wduplicated-branches -Wnull-dereference -Wold-style-cast -Wnon-virtual-dtor -Woverloaded-virtual")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g")
endif()

add_subdirectory(lib)

add_subdirectory(dirs)
add_subdirectory(program)

