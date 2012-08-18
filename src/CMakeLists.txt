include (GenerateExportHeader)
add_compiler_export_flags()

# TODO(acm): From where?
# include (FullRPath)

# TODO(acm): Conditionalize
# find_package (Boost 1.50.0 COMPONENTS system program_options)
# include_directories (${Boost_INCLUDE_DIR})

add_subdirectory (lib)
# add_subdirectory (bin)
add_subdirectory (third_party)
add_subdirectory (test)
