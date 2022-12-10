#----------------------------------------------------------------
# Generated CMake target import file.
#----------------------------------------------------------------

# Commands may need to know the format version.
set(CMAKE_IMPORT_FILE_VERSION 1)

# Import target "dgm_fr3_dyn::dgm_fr3_dyn" for configuration ""
set_property(TARGET dgm_fr3_dyn::dgm_fr3_dyn APPEND PROPERTY IMPORTED_CONFIGURATIONS NOCONFIG)
set_target_properties(dgm_fr3_dyn::dgm_fr3_dyn PROPERTIES
  IMPORTED_LOCATION_NOCONFIG "${_IMPORT_PREFIX}/lib/libdgm_fr3_dyn.so"
  IMPORTED_SONAME_NOCONFIG "libdgm_fr3_dyn.so"
  )

list(APPEND _IMPORT_CHECK_TARGETS dgm_fr3_dyn::dgm_fr3_dyn )
list(APPEND _IMPORT_CHECK_FILES_FOR_dgm_fr3_dyn::dgm_fr3_dyn "${_IMPORT_PREFIX}/lib/libdgm_fr3_dyn.so" )

# Import target "dgm_fr3_dyn::dgm_fr3_dyn_main" for configuration ""
set_property(TARGET dgm_fr3_dyn::dgm_fr3_dyn_main APPEND PROPERTY IMPORTED_CONFIGURATIONS NOCONFIG)
set_target_properties(dgm_fr3_dyn::dgm_fr3_dyn_main PROPERTIES
  IMPORTED_LOCATION_NOCONFIG "${_IMPORT_PREFIX}/lib/dgm_fr3_dyn/dgm_fr3_dyn_main"
  )

list(APPEND _IMPORT_CHECK_TARGETS dgm_fr3_dyn::dgm_fr3_dyn_main )
list(APPEND _IMPORT_CHECK_FILES_FOR_dgm_fr3_dyn::dgm_fr3_dyn_main "${_IMPORT_PREFIX}/lib/dgm_fr3_dyn/dgm_fr3_dyn_main" )

# Commands beyond this point should not need to know the version.
set(CMAKE_IMPORT_FILE_VERSION)
