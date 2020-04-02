cmake_minimum_required (VERSION 3.5)

project (gnucash_addon
  VERSION 0.1
)

cmake_minimum_required(VERSION 3.5)

# SET(CMAKE_BUILD_TYPE "Debug")

set(GNC_SRC_DIR ${PROJECT_SOURCE_DIR}/gnucash)
set(GNC_LIBGNC_SOURCE_DIR ${GNC_SRC_DIR}/libgnucash)
set(PAT_SOURCE_DIR pat)


option (WITH_SQL "Build this project with SQL (libdbi) support" OFF)
option (WITH_AQBANKING "Build this project with aqbanking (online banking) support" OFF)
option (WITH_GNUCASH "Build all of GnuCash, not just the library" OFF)
option (WITH_OFX "compile with ofx support (needs LibOFX)" OFF)
option (WITH_PYTHON "enable python plugin and bindings" OFF)
option (GENERATE_SWIG_WRAPPERS "SWIG" ON)

add_subdirectory(${GNC_SRC_DIR})
add_subdirectory(pat)
add_subdirectory(entry)


