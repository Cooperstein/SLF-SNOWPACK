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


SET(CPACK_ARCH "x86_64")
SET(CPACK_ARCHIVE_COMPONENT_INSTALL "OFF")
SET(CPACK_BINARY_7Z "")
SET(CPACK_BINARY_BUNDLE "")
SET(CPACK_BINARY_CYGWIN "")
SET(CPACK_BINARY_DEB "")
SET(CPACK_BINARY_DRAGNDROP "")
SET(CPACK_BINARY_IFW "")
SET(CPACK_BINARY_NSIS "")
SET(CPACK_BINARY_OSXX11 "")
SET(CPACK_BINARY_PACKAGEMAKER "")
SET(CPACK_BINARY_PRODUCTBUILD "")
SET(CPACK_BINARY_RPM "")
SET(CPACK_BINARY_STGZ "")
SET(CPACK_BINARY_TBZ2 "")
SET(CPACK_BINARY_TGZ "")
SET(CPACK_BINARY_TXZ "")
SET(CPACK_BINARY_TZ "")
SET(CPACK_BINARY_WIX "")
SET(CPACK_BINARY_ZIP "")
SET(CPACK_BUILD_SOURCE_DIRS "/home/nander/snowpack_greenland_new;/home/nander/snowpack_greenland_new")
SET(CPACK_BUNDLE_ALL "OFF")
SET(CPACK_CMAKE_GENERATOR "Unix Makefiles")
SET(CPACK_COMPONENTS_ALL "documentation;examples;exe")
SET(CPACK_COMPONENTS_ALL_SET_BY_USER "TRUE")
SET(CPACK_COMPONENT_EXE_DESCRIPTION "The SNOWPACK standalone model")
SET(CPACK_COMPONENT_EXE_DISPLAY_NAME "Snowpack application")
SET(CPACK_COMPONENT_EXE_GROUP "Runtime")
SET(CPACK_COMPONENT_GROUP_RUNTIME_DESCRIPTION "What you need to run an application based on SNOWPACK")
SET(CPACK_COMPONENT_LIBRARIES_DESCRIPTION "The SNOWPACK static library")
SET(CPACK_COMPONENT_LIBRARIES_DISPLAY_NAME "Snowpack static library")
SET(CPACK_COMPONENT_RUNTIME_GROUP "Runtime")
SET(CPACK_COMPONENT_UNSPECIFIED_HIDDEN "TRUE")
SET(CPACK_COMPONENT_UNSPECIFIED_REQUIRED "TRUE")
SET(CPACK_DEBIAN_PACKAGE_ARCHITECTURE "amd64")
SET(CPACK_DEBIAN_PACKAGE_DEPENDS "libstdc++, meteoio")
SET(CPACK_DEBIAN_PACKAGE_NAME "snowpack")
SET(CPACK_DEBIAN_PACKAGE_SECTION "science")
SET(CPACK_GENERATOR "DEB;RPM;TGZ")
SET(CPACK_INSTALL_CMAKE_PROJECTS "/home/nander/snowpack_greenland_new;snowpack;ALL;/")
SET(CPACK_INSTALL_PREFIX "/home/nander/usr")
SET(CPACK_MODULE_PATH "/home/nander/snowpack_greenland_new/tools/cmake/")
SET(CPACK_NSIS_DISPLAY_NAME "Snowpack")
SET(CPACK_NSIS_INSTALLER_ICON_CODE "")
SET(CPACK_NSIS_INSTALLER_MUI_ICON_CODE "")
SET(CPACK_NSIS_INSTALL_ROOT "$PROGRAMFILES")
SET(CPACK_NSIS_PACKAGE_NAME "Snowpack")
SET(CPACK_OUTPUT_CONFIG_FILE "/home/nander/snowpack_greenland_new/CPackConfig.cmake")
SET(CPACK_PACKAGE_CONTACT "Mathias Bavay <Mathias.Bavay@wsl.ch>")
SET(CPACK_PACKAGE_DEFAULT_LOCATION "/")
SET(CPACK_PACKAGE_DESCRIPTION "Snowpack Model")
SET(CPACK_PACKAGE_DESCRIPTION_FILE "/home/nander/snowpack_greenland_new/Welcome.txt")
SET(CPACK_PACKAGE_DESCRIPTION_SUMMARY "This is the SNOWPACK model ([Bartelt 2002], [Lehning 2002a], [Lehning 2002b]) for physical modeling of snow pack. This consists of a library and a program calling it so it can easily be integrated into other models.")
SET(CPACK_PACKAGE_FILE_NAME "Snowpack-3.4.1-x86_64")
SET(CPACK_PACKAGE_ICON "/home/nander/snowpack_greenland_new/snowpack/icons\\icon.ico")
SET(CPACK_PACKAGE_INSTALL_DIRECTORY "Snowpack")
SET(CPACK_PACKAGE_INSTALL_REGISTRY_KEY "Snowpack")
SET(CPACK_PACKAGE_NAME "Snowpack")
SET(CPACK_PACKAGE_RELOCATABLE "true")
SET(CPACK_PACKAGE_VENDOR "WSL Institute for Snow and Avalanche Research")
SET(CPACK_PACKAGE_VERSION "3.4.1")
SET(CPACK_PACKAGE_VERSION_MAJOR "3")
SET(CPACK_PACKAGE_VERSION_MINOR "4")
SET(CPACK_PACKAGE_VERSION_PATCH "1")
SET(CPACK_PACKAGING_INSTALL_PREFIX "/usr")
SET(CPACK_RESOURCE_FILE_LICENSE "/home/nander/snowpack_greenland_new/License.txt")
SET(CPACK_RESOURCE_FILE_README "/home/nander/snowpack_greenland_new/Readme.txt")
SET(CPACK_RESOURCE_FILE_WELCOME "/home/nander/snowpack_greenland_new/Welcome.txt")
SET(CPACK_RPM_PACKAGE_ARCHITECTURE "x86_64")
SET(CPACK_RPM_PACKAGE_GROUP "Applications/Science")
SET(CPACK_RPM_PACKAGE_LICENSE "LPGLv3")
SET(CPACK_RPM_PACKAGE_NAME "snowpack")
SET(CPACK_RPM_PACKAGE_REQUIRES "libstdc++6, meteoio")
SET(CPACK_SET_DESTDIR "OFF")
SET(CPACK_SOURCE_7Z "")
SET(CPACK_SOURCE_CYGWIN "")
SET(CPACK_SOURCE_GENERATOR "TGZ")
SET(CPACK_SOURCE_IGNORE_FILES "~$;\\.svn/;/CMakeFiles/;/CMakeCache*;/cmake_install\\.cmake$;/cmake_uninstall\\.cmake$;^_CPack_.*/;/CPack*;\\.o$;/m4/")
SET(CPACK_SOURCE_OUTPUT_CONFIG_FILE "/home/nander/snowpack_greenland_new/CPackSourceConfig.cmake")
SET(CPACK_SOURCE_PACKAGE_FILE_NAME "Snowpack-3.4.1")
SET(CPACK_SOURCE_RPM "")
SET(CPACK_SOURCE_TBZ2 "")
SET(CPACK_SOURCE_TGZ "")
SET(CPACK_SOURCE_TXZ "")
SET(CPACK_SOURCE_TZ "")
SET(CPACK_SOURCE_ZIP "")
SET(CPACK_SYSTEM_NAME "Linux")
SET(CPACK_TOPLEVEL_TAG "Linux")
SET(CPACK_WIX_SIZEOF_VOID_P "8")

if(NOT CPACK_PROPERTIES_FILE)
  set(CPACK_PROPERTIES_FILE "/home/nander/snowpack_greenland_new/CPackProperties.cmake")
endif()

if(EXISTS ${CPACK_PROPERTIES_FILE})
  include(${CPACK_PROPERTIES_FILE})
endif()
