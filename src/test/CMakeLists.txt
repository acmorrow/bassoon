include_directories (${PROJECT_SOURCE_DIR}/src/lib)
include_directories (${PROJECT_BINARY_DIR}/src/lib)

include_directories (${PROJECT_SOURCE_DIR}/src/third_party/googletest/src/include)

macro (create_test suite_name test_name)
  add_executable (${test_name} ${test_name}.cpp)
  target_link_libraries (${test_name} libbassoon gtest gtest_main)
  install (TARGETS ${test_name} DESTINATION bin/tests COMPONENT tests)
  add_test (${suite_name}.${test_name} ${test_name})
endmacro ()

macro (create_tests suite_name)
  foreach (test_name ${ARGN})
    create_test (${suite_name} ${test_name})
  endforeach ()
endmacro ()

create_tests (libbassoon
  test_config
  test_encode_hello_world
)



