set (libbassoon_version_major 0)
set (libbassoon_version_minor 1)
set (libbassoon_version_patch 0)

include_directories (${PROJECT_SOURCE_DIR}/src/lib)
include_directories (${PROJECT_BINARY_DIR}/src/lib)

file (GLOB libbassoon_headers *.h *.hpp)
file (GLOB libbassoon_sources *.cpp)

configure_file ("config.h.in" "config.h")
list (APPEND libbassoon_headers ${CMAKE_CURRENT_BINARY_DIR}/config.h)

add_library (libbassoon ${libbassoon_sources})

set_target_properties (libbassoon PROPERTIES
  OUTPUT_NAME bassoon
  DEFINE_SYMBOL LIBBASSOON_EXPORTS
  VERSION "${libbassoon_version_major}.${libbassoon_version_minor}.${libbassoon_version_patch}")

# TODO(acm): Update g_e_h to let you customize the include guard name.
generate_export_header (libbassoon
  EXPORT_FILE_NAME export.h)
list (APPEND libbassoon_headers ${CMAKE_CURRENT_BINARY_DIR}/export.h)

install (TARGETS libbassoon DESTINATION lib)
install (FILES ${libbassoon_headers} DESTINATION include/bassoon)
