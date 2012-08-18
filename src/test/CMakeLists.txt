include_directories (${PROJECT_SOURCE_DIR}/src/lib)
include_directories (${PROJECT_BINARY_DIR}/src/lib)

include_directories (${PROJECT_SOURCE_DIR}/src/third_party/googletest/src/include)

# NOTE(acm): This is to work around building google-test from macports
# in C++11 mode wth libc++, which fails to compile without this
# option. We need to pass those same options here.
add_definitions (-DGTEST_HAS_TR1_TUPLE=0 -DGTEST_USE_OWN_TR1_TUPLE)

macro (create_test suite_name test_name)
  add_executable (${test_name} ${test_name}.cpp)
  target_link_libraries (${test_name} libbassoon gtest gtest_main)
  add_test (${suite_name}.${test_name} ${test_name})
endmacro ()

macro (create_tests suite_name)
  foreach (test_name ${ARGN})
    create_test (${suite_name} ${test_name})
  endforeach ()
endmacro ()

create_tests (libbassoon
  test_config
)
