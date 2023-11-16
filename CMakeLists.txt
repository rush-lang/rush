cmake_minimum_required(VERSION 3.11)
set(RUSH_VERSION_MAJOR 0 CACHE STRING "major version" FORCE)
set(RUSH_VERSION_MINOR 0 CACHE STRING "minor version" FORCE)
set(RUSH_VERSION_PATCH 0 CACHE STRING "patch version" FORCE)
set(RUSH_VERSION ${RUSH_VERSION_MAJOR}.${RUSH_VERSION_MINOR}.${RUSH_VERSION_PATCH} CACHE STRING "version" FORCE)

set(RUNTIME_DEST bin)
set(LIBRARY_DEST lib/rush-${RUSH_VERSION_MAJOR})
set(INCLUDE_DEST include)

option(RUSH_BUILD_TOOLS "Generate build targets for Rush tools." ON)
option(RUSH_BUILD_TESTS "Generate build targets for Rush unit tests." ON)
option(RUSH_BUILD_STATIC_LIBS "Generate build targets for static libraries." ON)
option(RUSH_BUILD_SHARED_LIBS "Generate build targets for shared libraries." OFF)

configure_file(include/rush/version.hpp.in include/rush/version.hpp)
project(rush VERSION ${RUSH_VERSION} LANGUAGES C CXX)
list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_LIST_DIR}/cmake/modules")

include(RushAdd)
include(BuildType)
include(GNUInstallDirs)

# TODO: find specific version so there's no difference between environments
find_package(LLVM REQUIRED CONFIG)
include_directories(${LLVM_INCLUDE_DIRS})
message(STATUS "Found LLVM ${LLVM_VERSION}")
message(STATUS "Using LLVMConfig.cmake in: ${LLVM_DIR}")

if(RUSH_BUILD_TESTS)
   find_package(Catch2 3 QUIET)
   if(Catch2_FOUND)
      message(STATUS "Found Catch2 ${Catch2_VERSION}")
      message(STATUS "Using Catch2onfig.cmake in: ${Catch2_DIR}")
      include(Catch)
   endif()
endif()

## Set the standard library implementation to use for clang.
if("${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang")
   set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -stdlib=libc++")
endif()

## Use CCache if available.
find_program(CCACHE_FOUND ccache)
if(CCACHE_FOUND)
   set_property(GLOBAL PROPERTY RULE_LAUNCH_COMPILE ccache)
endif(CCACHE_FOUND)

## add subdirectories to generate core targets
add_subdirectory(external)
add_subdirectory(utils)
add_subdirectory(lib)

## add subdirectories to build tools if enabled
if(RUSH_BUILD_TOOLS)
   add_subdirectory(tools)
endif()

## install rush headers
install(DIRECTORY include/rush
   DESTINATION ${INCLUDE_DEST}
   FILES_MATCHING PATTERN "*.h??")

## install cmake configured header files
install(DIRECTORY ${CMAKE_BINARY_DIR}/include/rush
   DESTINATION ${INCLUDE_DEST}
   FILES_MATCHING PATTERN "*.h??")

## install libary targets
install(TARGETS
   ast
   core
   diag
   lexer
   parser
   sema
   irgen
   LIBRARY DESTINATION ${LIBRARY_DEST})

## install tool targets if enabled
if(RUSH_BUILD_TOOLS)
   install(TARGETS
      crush
      rupl
      RUNTIME DESTINATION ${RUNTIME_DEST})
endif()

## build tests if enabled
if(RUSH_BUILD_TESTS)
   include(CTest)
   enable_testing()
   add_subdirectory(test)
endif()
