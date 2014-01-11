include (FullRPath)

# TODO(acm): This doesn't seem to work?
set(INTERPROCEDURAL_OPTIMIZATION 1)
set(INTERPROCEDURAL_OPTIMIZATION_RELWITHDEBINFO 1)

add_subdirectory (lib)
add_subdirectory (bin)
add_subdirectory (test)
add_subdirectory (third_party)
