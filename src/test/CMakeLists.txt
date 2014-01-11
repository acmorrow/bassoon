# Get and build gtest

include(ExternalProject)
set_directory_properties(PROPERTIES EP_PREFIX ${CMAKE_BINARY_DIR}/src/third_party)
ExternalProject_Add(
    googletest
    URL http://googletest.googlecode.com/files/gtest-1.7.0.zip
    INSTALL_COMMAND ""
    LOG_DOWNLOAD ON
    LOG_CONFIGURE ON
    LOG_BUILD ON

    CMAKE_ARGS -DCMAKE_BUILD_TYPE=${CMAKE_BUILD_TYPE}
    -DBUILD_SHARED_LIBS=${BUILD_SHARED_LIBS}
    -Dgtest_force_shared_crt=ON
)

# Add include dir
ExternalProject_Get_Property(googletest source_dir)
include_directories(${source_dir}/include)

# Add library dir
ExternalProject_Get_Property(googletest binary_dir)
link_directories(${binary_dir})

macro (create_test suite_name test_name)
  add_executable (${test_name} ${test_name}.cpp)
  add_dependencies (${test_name} googletest)
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
  test_encode_hello_world
)
