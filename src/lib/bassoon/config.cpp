#include <bassoon/config.hpp>

void bassoon_get_library_version(size_t* major, size_t* minor) {
  if (major)
    *major = BASSOON_VERSION_MAJOR;
  if (minor)
    *minor = BASSOON_VERSION_MINOR;
}
