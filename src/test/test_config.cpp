#include <bassoon/config.hpp>

#include <gtest/gtest.h>

namespace {

  // Check that calling get_library_version with no arguments
  // works OK.
  TEST(ConfigTest, GetVersionNullArgs) {
    bassoon_get_library_version(NULL, NULL);
  }

  // Check that get_library_version gives back the major version OK.
  TEST(ConfigTest, TestGetMajorVersion) {
    size_t version_major = BASSOON_VERSION_MAJOR + 1;
    bassoon_get_library_version(&version_major, NULL);
    EXPECT_EQ(BASSOON_VERSION_MAJOR, version_major);
  }

  // Check that get_library_version gives back the minor version OK.
  TEST(ConfigTest, TestGetMinorVersion) {
    size_t version_minor = BASSOON_VERSION_MINOR + 1;
    bassoon_get_library_version(NULL, &version_minor);
    EXPECT_EQ(BASSOON_VERSION_MINOR, version_minor);
  }

} // namespace
