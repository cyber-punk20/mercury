# This file will be configured to contain variables for CPack. These variables
# should be set in the CMake list file of the project before CPack module is
# included. The list of available CPACK_xxx variables and their associated
# documentation may be obtained using
#  cpack --help-variable-list
#
# Some variables are common to all generators (e.g. CPACK_PACKAGE_NAME)
# and some are specific to a generator
# (e.g. CPACK_NSIS_EXTRA_INSTALL_COMMANDS). The generator specific variables
# usually begin with CPACK_<GENNAME>_xxxx.


set(CPACK_BUILD_SOURCE_DIRS "/work2/08571/yhkan/frontera/mercury;/work2/08571/yhkan/frontera/mercury/Testing")
set(CPACK_CMAKE_GENERATOR "Unix Makefiles")
set(CPACK_COMPONENT_UNSPECIFIED_HIDDEN "TRUE")
set(CPACK_COMPONENT_UNSPECIFIED_REQUIRED "TRUE")
set(CPACK_DEFAULT_PACKAGE_DESCRIPTION_FILE "/opt/apps/cmake/3.24.2/share/cmake-3.24/Templates/CPack.GenericDescription.txt")
set(CPACK_DEFAULT_PACKAGE_DESCRIPTION_SUMMARY "MERCURY built using CMake")
set(CPACK_DMG_SLA_USE_RESOURCE_FILE_LICENSE "ON")
set(CPACK_GENERATOR "TBZ2")
set(CPACK_INSTALL_CMAKE_PROJECTS "/work2/08571/yhkan/frontera/mercury/Testing;MERCURY;ALL;/")
set(CPACK_INSTALL_PREFIX "/usr/local")
set(CPACK_MODULE_PATH "/work2/08571/yhkan/frontera/mercury/CMake")
set(CPACK_NSIS_DISPLAY_NAME "Mercury 2.2.0")
set(CPACK_NSIS_INSTALLER_ICON_CODE "")
set(CPACK_NSIS_INSTALLER_MUI_ICON_CODE "")
set(CPACK_NSIS_INSTALL_ROOT "$PROGRAMFILES")
set(CPACK_NSIS_PACKAGE_NAME "Mercury 2.2.0")
set(CPACK_NSIS_UNINSTALL_NAME "Uninstall")
set(CPACK_OUTPUT_CONFIG_FILE "/work2/08571/yhkan/frontera/mercury/Testing/CPackConfig.cmake")
set(CPACK_PACKAGE_DEFAULT_LOCATION "/")
set(CPACK_PACKAGE_DESCRIPTION_FILE "/work2/08571/yhkan/frontera/mercury/README.md")
set(CPACK_PACKAGE_DESCRIPTION_SUMMARY "RPC for High-Performance Computing")
set(CPACK_PACKAGE_FILE_NAME "Mercury-2.2.0-Linux")
set(CPACK_PACKAGE_INSTALL_DIRECTORY "Mercury 2.2.0")
set(CPACK_PACKAGE_INSTALL_REGISTRY_KEY "Mercury 2.2.0")
set(CPACK_PACKAGE_NAME "Mercury")
set(CPACK_PACKAGE_RELOCATABLE "true")
set(CPACK_PACKAGE_VENDOR "Argonne National Laboratory / The HDF Group")
set(CPACK_PACKAGE_VERSION "2.2.0")
set(CPACK_PACKAGE_VERSION_MAJOR "2")
set(CPACK_PACKAGE_VERSION_MINOR "2")
set(CPACK_PACKAGE_VERSION_PATCH "0")
set(CPACK_RESOURCE_FILE_LICENSE "/work2/08571/yhkan/frontera/mercury/LICENSE.txt")
set(CPACK_RESOURCE_FILE_README "/opt/apps/cmake/3.24.2/share/cmake-3.24/Templates/CPack.GenericDescription.txt")
set(CPACK_RESOURCE_FILE_WELCOME "/opt/apps/cmake/3.24.2/share/cmake-3.24/Templates/CPack.GenericWelcome.txt")
set(CPACK_SET_DESTDIR "OFF")
set(CPACK_SOURCE_GENERATOR "TBZ2")
set(CPACK_SOURCE_IGNORE_FILES "/\\.git/;/\\.git$;/\\.gitattributes$;/\\.github/;/\\.gitignore$;/\\.gitmodules$;/\\.vscode/;/build/;\\.swp$;\\.#;/#;~$")
set(CPACK_SOURCE_OUTPUT_CONFIG_FILE "/work2/08571/yhkan/frontera/mercury/Testing/CPackSourceConfig.cmake")
set(CPACK_SOURCE_PACKAGE_FILE_NAME "mercury-2.2.0-8-g280c750")
set(CPACK_SOURCE_STRIP_FILES "")
set(CPACK_SYSTEM_NAME "Linux")
set(CPACK_THREADS "1")
set(CPACK_TOPLEVEL_TAG "Linux")
set(CPACK_WIX_SIZEOF_VOID_P "8")

if(NOT CPACK_PROPERTIES_FILE)
  set(CPACK_PROPERTIES_FILE "/work2/08571/yhkan/frontera/mercury/Testing/CPackProperties.cmake")
endif()

if(EXISTS ${CPACK_PROPERTIES_FILE})
  include(${CPACK_PROPERTIES_FILE})
endif()
