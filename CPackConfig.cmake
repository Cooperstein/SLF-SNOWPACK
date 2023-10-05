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


set(CPACK_ARCH "x86_64")
set(CPACK_ARCHIVE_COMPONENT_INSTALL "OFF")
set(CPACK_BINARY_7Z "")
set(CPACK_BINARY_BUNDLE "")
set(CPACK_BINARY_CYGWIN "")
set(CPACK_BINARY_DEB "")
set(CPACK_BINARY_DRAGNDROP "")
set(CPACK_BINARY_FREEBSD "")
set(CPACK_BINARY_IFW "")
set(CPACK_BINARY_NSIS "")
set(CPACK_BINARY_NUGET "")
set(CPACK_BINARY_OSXX11 "")
set(CPACK_BINARY_PACKAGEMAKER "")
set(CPACK_BINARY_PRODUCTBUILD "")
set(CPACK_BINARY_RPM "")
set(CPACK_BINARY_STGZ "")
set(CPACK_BINARY_TBZ2 "")
set(CPACK_BINARY_TGZ "")
set(CPACK_BINARY_TXZ "")
set(CPACK_BINARY_TZ "")
set(CPACK_BINARY_WIX "")
set(CPACK_BINARY_ZIP "")
set(CPACK_BUILD_SOURCE_DIRS "/home/varun/WRF/install_folder/code/snpack_for_wrf/snowpack;/home/varun/WRF/install_folder/code/snpack_for_wrf/snowpack")
set(CPACK_BUNDLE_ALL "OFF")
set(CPACK_CMAKE_GENERATOR "Unix Makefiles")
set(CPACK_COMPONENTS_ALL "documentation;examples;exe")
set(CPACK_COMPONENTS_ALL_SET_BY_USER "TRUE")
set(CPACK_COMPONENT_EXE_DESCRIPTION "The SNOWPACK standalone model")
set(CPACK_COMPONENT_EXE_DISPLAY_NAME "Snowpack application")
set(CPACK_COMPONENT_EXE_GROUP "Runtime")
set(CPACK_COMPONENT_GROUP_RUNTIME_DESCRIPTION "What you need to run an application based on SNOWPACK")
set(CPACK_COMPONENT_LIBRARIES_DESCRIPTION "The SNOWPACK library")
set(CPACK_COMPONENT_LIBRARIES_DISPLAY_NAME "Snowpack library")
set(CPACK_COMPONENT_RUNTIME_GROUP "Runtime")
set(CPACK_COMPONENT_UNSPECIFIED_HIDDEN "TRUE")
set(CPACK_COMPONENT_UNSPECIFIED_REQUIRED "TRUE")
set(CPACK_DEBIAN_PACKAGE_ARCHITECTURE "amd64")
set(CPACK_DEBIAN_PACKAGE_DEPENDS "libstdc++6, libc6")
set(CPACK_DEBIAN_PACKAGE_NAME "snowpack")
set(CPACK_DEBIAN_PACKAGE_SECTION "science")
set(CPACK_DEFAULT_PACKAGE_DESCRIPTION_FILE "/usr/share/cmake-3.16/Templates/CPack.GenericDescription.txt")
set(CPACK_GENERATOR "DEB;RPM;TGZ")
set(CPACK_INSTALL_CMAKE_PROJECTS "/home/varun/WRF/install_folder/code/snpack_for_wrf/snowpack;snowpack;ALL;/")
set(CPACK_INSTALL_PREFIX "/home/varun/WRF/install_folder/code/snpack_for_wrf/snow_libs")
set(CPACK_MODULE_PATH "/home/varun/WRF/install_folder/code/snpack_for_wrf/snowpack/tools/cmake/")
set(CPACK_NSIS_DISPLAY_NAME "Snowpack")
set(CPACK_NSIS_INSTALLER_ICON_CODE "")
set(CPACK_NSIS_INSTALLER_MUI_ICON_CODE "")
set(CPACK_NSIS_INSTALL_ROOT "$PROGRAMFILES")
set(CPACK_NSIS_PACKAGE_NAME "Snowpack")
set(CPACK_OUTPUT_CONFIG_FILE "/home/varun/WRF/install_folder/code/snpack_for_wrf/snowpack/CPackConfig.cmake")
set(CPACK_PACKAGE_CONTACT "Mathias Bavay <Mathias.Bavay@wsl.ch>")
set(CPACK_PACKAGE_DEFAULT_LOCATION "/")
set(CPACK_PACKAGE_DESCRIPTION "Snowpack Model")
set(CPACK_PACKAGE_DESCRIPTION_FILE "/home/varun/WRF/install_folder/code/snpack_for_wrf/snowpack/Welcome.txt")
set(CPACK_PACKAGE_DESCRIPTION_SUMMARY "This is the SNOWPACK model ([Bartelt 2002], [Lehning 2002a], [Lehning 2002b]) for physical modeling of snow pack. This consists of a library and a program calling it so it can easily be integrated into other models.")
set(CPACK_PACKAGE_FILE_NAME "Snowpack-3.4.5-x86_64")
set(CPACK_PACKAGE_ICON "/home/varun/WRF/install_folder/code/snpack_for_wrf/snowpack/snowpack/icons\\icon.ico")
set(CPACK_PACKAGE_INSTALL_DIRECTORY "Snowpack")
set(CPACK_PACKAGE_INSTALL_REGISTRY_KEY "Snowpack")
set(CPACK_PACKAGE_NAME "Snowpack")
set(CPACK_PACKAGE_RELOCATABLE "true")
set(CPACK_PACKAGE_VENDOR "WSL Institute for Snow and Avalanche Research")
set(CPACK_PACKAGE_VERSION "3.4.5")
set(CPACK_PACKAGE_VERSION_MAJOR "3")
set(CPACK_PACKAGE_VERSION_MINOR "4")
set(CPACK_PACKAGE_VERSION_PATCH "5")
set(CPACK_PACKAGING_INSTALL_PREFIX "/usr")
set(CPACK_RESOURCE_FILE_LICENSE "/home/varun/WRF/install_folder/code/snpack_for_wrf/snowpack/License.txt")
set(CPACK_RESOURCE_FILE_README "/home/varun/WRF/install_folder/code/snpack_for_wrf/snowpack/Readme.txt")
set(CPACK_RESOURCE_FILE_WELCOME "/home/varun/WRF/install_folder/code/snpack_for_wrf/snowpack/Welcome.txt")
set(CPACK_RPM_PACKAGE_ARCHITECTURE "x86_64")
set(CPACK_RPM_PACKAGE_GROUP "Applications/Science")
set(CPACK_RPM_PACKAGE_LICENSE "LPGLv3")
set(CPACK_RPM_PACKAGE_NAME "snowpack")
set(CPACK_RPM_PACKAGE_REQUIRES "libstdc++6, meteoio")
set(CPACK_SET_DESTDIR "OFF")
set(CPACK_SOURCE_7Z "")
set(CPACK_SOURCE_CYGWIN "")
set(CPACK_SOURCE_GENERATOR "TGZ")
set(CPACK_SOURCE_IGNORE_FILES "~$;\\.svn/;/CMakeFiles/;/CMakeCache*;/cmake_install\\.cmake$;/cmake_uninstall\\.cmake$;^_CPack_.*/;/CPack*;\\.o$;/m4/")
set(CPACK_SOURCE_OUTPUT_CONFIG_FILE "/home/varun/WRF/install_folder/code/snpack_for_wrf/snowpack/CPackSourceConfig.cmake")
set(CPACK_SOURCE_PACKAGE_FILE_NAME "Snowpack-3.4.5")
set(CPACK_SOURCE_RPM "")
set(CPACK_SOURCE_TBZ2 "")
set(CPACK_SOURCE_TGZ "")
set(CPACK_SOURCE_TXZ "")
set(CPACK_SOURCE_TZ "")
set(CPACK_SOURCE_ZIP "")
set(CPACK_SYSTEM_NAME "Linux")
set(CPACK_TOPLEVEL_TAG "Linux")
set(CPACK_WIX_SIZEOF_VOID_P "8")

if(NOT CPACK_PROPERTIES_FILE)
  set(CPACK_PROPERTIES_FILE "/home/varun/WRF/install_folder/code/snpack_for_wrf/snowpack/CPackProperties.cmake")
endif()

if(EXISTS ${CPACK_PROPERTIES_FILE})
  include(${CPACK_PROPERTIES_FILE})
endif()
