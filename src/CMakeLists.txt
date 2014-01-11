include (FullRPath)

set (BUILD_SHARED_LIBS ON)

# TODO(acm): Assuming GCC or clang for now.
set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Werror")

# This warning happens in binary_data but it is in a template
# so just easier to suppress.
set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wno-tautological-compare")

set (CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -O3 -flto")
set (CMAKE_CXX_FLAGS_RELWITHDEBINFO "${CMAKE_CXX_FLAGS_RELWITHDEBINFO} -O3 -flto")

# TODO(acm): This doesn't seem to work?
set (INTERPROCEDURAL_OPTIMIZATION_RELEASE ON)
set (INTERPROCEDURAL_OPTIMIZATION_RELWITHDEBINFO ON)

add_subdirectory (lib)
add_subdirectory (bin)
add_subdirectory (test)
