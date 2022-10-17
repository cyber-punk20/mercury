#----------------------------------------------------------------
# Generated CMake target import file for configuration "RelWithDebInfo".
#----------------------------------------------------------------

# Commands may need to know the format version.
set(CMAKE_IMPORT_FILE_VERSION 1)

# Import target "mercury_util" for configuration "RelWithDebInfo"
set_property(TARGET mercury_util APPEND PROPERTY IMPORTED_CONFIGURATIONS RELWITHDEBINFO)
set_target_properties(mercury_util PROPERTIES
  IMPORTED_LINK_INTERFACE_LANGUAGES_RELWITHDEBINFO "C"
  IMPORTED_LOCATION_RELWITHDEBINFO "/usr/local/lib/libmercury_util.a"
  )

list(APPEND _cmake_import_check_targets mercury_util )
list(APPEND _cmake_import_check_files_for_mercury_util "/usr/local/lib/libmercury_util.a" )

# Import target "na" for configuration "RelWithDebInfo"
set_property(TARGET na APPEND PROPERTY IMPORTED_CONFIGURATIONS RELWITHDEBINFO)
set_target_properties(na PROPERTIES
  IMPORTED_LINK_INTERFACE_LANGUAGES_RELWITHDEBINFO "C"
  IMPORTED_LOCATION_RELWITHDEBINFO "/usr/local/lib/libna.a"
  )

list(APPEND _cmake_import_check_targets na )
list(APPEND _cmake_import_check_files_for_na "/usr/local/lib/libna.a" )

# Import target "mercury" for configuration "RelWithDebInfo"
set_property(TARGET mercury APPEND PROPERTY IMPORTED_CONFIGURATIONS RELWITHDEBINFO)
set_target_properties(mercury PROPERTIES
  IMPORTED_LINK_INTERFACE_LANGUAGES_RELWITHDEBINFO "C"
  IMPORTED_LOCATION_RELWITHDEBINFO "/usr/local/lib/libmercury.a"
  )

list(APPEND _cmake_import_check_targets mercury )
list(APPEND _cmake_import_check_files_for_mercury "/usr/local/lib/libmercury.a" )

# Commands beyond this point should not need to know the version.
set(CMAKE_IMPORT_FILE_VERSION)
