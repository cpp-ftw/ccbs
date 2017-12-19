cmake_minimum_required (VERSION 2.6)

add_subdirectory(./../lib ccbs_lib)

include_directories(${SHARED_LIB_INCLUDE_DIR})

add_executable(ccbs main.cpp)
target_link_libraries(ccbs PUBLIC ccbs_lib ccsh_lib ${filesystem_lib})

add_executable(ccbs-debug debug.cpp)
add_compile_options(ccbs-debug -Wno-unknown-pragmas)
target_link_libraries(ccbs-debug PUBLIC ccbs_lib ccsh_lib ${filesystem_lib})

