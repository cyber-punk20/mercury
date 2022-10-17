#----------------------------------------------------------------
# Generated CMake target import file for configuration "RelWithDebInfo".
#----------------------------------------------------------------

# Commands may need to know the format version.
set(CMAKE_IMPORT_FILE_VERSION 1)

# Import target "mercury_util" for configuration "RelWithDebInfo"
set_property(TARGET mercury_util APPEND PROPERTY IMPORTED_CONFIGURATIONS RELWITHDEBINFO)
set_target_properties(mercury_util PROPERTIES
  IMPORTED_LOCATION_RELWITHDEBINFO "/work2/08571/yhkan/frontera/mercury/install/lib/libmercury_util.so.3.0.0"
  IMPORTED_SONAME_RELWITHDEBINFO "libmercury_util.so.3"
  )

list(APPEND _cmake_import_check_targets mercury_util )
list(APPEND _cmake_import_check_files_for_mercury_util "/work2/08571/yhkan/frontera/mercury/install/lib/libmercury_util.so.3.0.0" )

# Import target "na" for configuration "RelWithDebInfo"
set_property(TARGET na APPEND PROPERTY IMPORTED_CONFIGURATIONS RELWITHDEBINFO)
set_target_properties(na PROPERTIES
  IMPORTED_LOCATION_RELWITHDEBINFO "/work2/08571/yhkan/frontera/mercury/install/lib/libna.so.3.0.0"
  IMPORTED_SONAME_RELWITHDEBINFO "libna.so.3"
  )

list(APPEND _cmake_import_check_targets na )
list(APPEND _cmake_import_check_files_for_na "/work2/08571/yhkan/frontera/mercury/install/lib/libna.so.3.0.0" )

# Import target "mercury" for configuration "RelWithDebInfo"
set_property(TARGET mercury APPEND PROPERTY IMPORTED_CONFIGURATIONS RELWITHDEBINFO)
set_target_properties(mercury PROPERTIES
  IMPORTED_LOCATION_RELWITHDEBINFO "/work2/08571/yhkan/frontera/mercury/install/lib/libmercury.so.2.2.0"
  IMPORTED_SONAME_RELWITHDEBINFO "libmercury.so.2"
  )

list(APPEND _cmake_import_check_targets mercury )
list(APPEND _cmake_import_check_files_for_mercury "/work2/08571/yhkan/frontera/mercury/install/lib/libmercury.so.2.2.0" )

# Commands beyond this point should not need to know the version.
set(CMAKE_IMPORT_FILE_VERSION)
