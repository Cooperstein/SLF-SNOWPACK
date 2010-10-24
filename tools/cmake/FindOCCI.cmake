# - Find Oracle OCCI library
# Find the Oracle OCCI library and includes
#
# It defines the following variables:
# OCCI_LIBRARY - the library
# OCCI_INCLUDE_DIR - the header
# OCCI_FOUND - Set to false, or undefined, if we haven't found

include(LibFindMacros)

# Finally the library itself
find_library(OCCI_LIBRARY
  NAMES occi clntsh
  HINTS $ENV{ORACLE_HOME}/client_1/lib $ENV{ORACLE_BASE}/client_1/lib
  PATHS "/usr/lib" "/usr/local/lib" "~/usr/lib" "/opt/lib" ENV LD_LIBRARY_PATH
  PATH_SUFFIXES "oracle/*/client_1/lib"
)

#build ORACLE_ROOT so we can provide a hint for searching for the header file
if ("${OCCI_LIBRARY}" MATCHES "^(.+)lib[\\/]libocci.(.+)$")
   set(ORACLE_ROOT "${CMAKE_MATCH_1}")
endif ("${OCCI_LIBRARY}" MATCHES "^(.+)lib[\\/]libocci.(.+)$")

# locate header files
find_path(OCCI_INCLUDE_DIR
  NAMES occi.h
  HINTS ${ORACLE_ROOT}/include
  PATHS "/usr/include" "/usr/local/include" "/usr/local" "~/usr/include" "/opt/include"
  PATH_SUFFIXES "oracle/*/client_1/include"
)

# Set the include dir variables and the libraries and let libfind_process do the rest.
# NOTE: Singular variables for this library, plural for libraries this this lib depends on.
set(OCCI_PROCESS_INCLUDES OCCI_INCLUDE_DIR)
set(OCCI_PROCESS_LIBS OCCI_LIBRARY)
libfind_process(OCCI)