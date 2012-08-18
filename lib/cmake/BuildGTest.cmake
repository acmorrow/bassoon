include (ExternalProject)

function(BuildGTest)

  ExternalProject_Add(
    googletest
    SVN_REPOSITORY http://googletest.googlecode.com/svn/trunk/
    CMAKE_ARGS -DBUILD_SHARED_LIBS:BOOL=${BUILD_SHARED_LIBS}
    TIMEOUT 10
    INSTALL_COMMAND ""
    # Wrap download, configure and build steps in a script to log output
    LOG_DOWNLOAD ON
    LOG_CONFIGURE ON
    LOG_BUILD ON)

  ExternalProject_Get_Property(googletest source_dir)
  ExternalProject_Get_Property(googletest binary_dir)

  # Set things up like find_package(GTest) would
  #
  # TODO(acm): How can we get rid of the LIB_DIRS and get the derived
  # library names?
  set (_GTEST_FOUND true)
  set (_GTEST_INCLUDE_DIRS ${source_dir}/include)
  set (_GTEST_LIB_DIRS ${binary_dir})
  set (_GTEST_LIBRARIES gtest)
  set (_GTEST_MAIN_LIBRARIES gtest_main)
  set (_GTEST_BOTH_LIBRARIES ${_GTEST_MAIN_LIBRARIES} ${_GTEST_LIBRARIES})

  set (GTEST_FOUND ${_GTEST_FOUND} PARENT_SCOPE)
  set (GTEST_INCLUDE_DIRS ${_GTEST_INCLUDE_DIRS} PARENT_SCOPE)
  set (GTEST_LIB_DIRS ${_GTEST_LIB_DIRS} PARENT_SCOPE)
  set (GTEST_LIBRARIES ${_GTEST_LIBRARIES} PARENT_SCOPE)
  set (GTEST_MAIN_LIBRARIES ${_GTEST_MAIN_LIBRARIES} PARENT_SCOPE)
  set (GTEST_BOTH_LIBRARIES ${_GTEST_BOTH_LIBRARIES} PARENT_SCOPE)

endfunction()
