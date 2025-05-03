#----------------------------------------------------------------
# Generated CMake target import file for configuration "Release".
#----------------------------------------------------------------

# Commands may need to know the format version.
set(CMAKE_IMPORT_FILE_VERSION 1)

# Import target "Mosaic::Mosaic" for configuration "Release"
set_property(TARGET Mosaic::Mosaic APPEND PROPERTY IMPORTED_CONFIGURATIONS RELEASE)
set_target_properties(Mosaic::Mosaic PROPERTIES
  IMPORTED_LINK_INTERFACE_LANGUAGES_RELEASE "CXX"
  IMPORTED_LOCATION_RELEASE "${_IMPORT_PREFIX}/lib/libMosaic.a"
  )

list(APPEND _cmake_import_check_targets Mosaic::Mosaic )
list(APPEND _cmake_import_check_files_for_Mosaic::Mosaic "${_IMPORT_PREFIX}/lib/libMosaic.a" )

# Commands beyond this point should not need to know the version.
set(CMAKE_IMPORT_FILE_VERSION)
