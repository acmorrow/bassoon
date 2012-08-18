cmake_minimum_required (VERSION 2.8)

project (bassoon)
set (BASSOON_VERSION_MAJOR 0)
set (BASSOON_VERSION_MINOR 1)
set (BASSOON_VERSION_PATCH 0)

set (CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/lib/cmake")
# TODO(acm): Clone my modules from somewhere?

include (InstallRequiredSystemLibraries)
set (CPACK_RESOURCE_FILE_LICENSE
     "${CMAKE_CURRENT_SOURCE_DIR}/LICENSE")
set (CPACK_PACKAGE_VERSION_MAJOR "${BASSOON_VERSION_MAJOR}")
set (CPACK_PACKAGE_VERSION_MINOR "${BASSOON_VERSION_MINOR}")
set (CPACK_PACKAGE_VERSION_MINOR "${BASSOON_VERSION_PATCH}")

include (CPack)

enable_testing()
add_subdirectory (src)
