cmake_minimum_required (VERSION 2.6)

add_subdirectory(./../lib ccbs_lib)

include_directories(${SHARED_LIB_INCLUDE_DIR})

add_executable(ccbs-dirs main.cpp)
target_link_libraries(ccbs-dirs PUBLIC ccbs_lib ccsh_lib ${filesystem_lib})
