cmake_minimum_required(VERSION 3.11)
set(RUSH_VERSION_MAJOR 0 CACHE STRING "major version" FORCE)
set(RUSH_VERSION_MINOR 0 CACHE STRING "minor version" FORCE)
set(RUSH_VERSION_PATCH 1 CACHE STRING "patch version" FORCE)
set(RUSH_VERSION ${RUSH_VERSION_MAJOR}.${RUSH_VERSION_MINOR}.${RUSH_VERSION_PATCH} CACHE STRING "version" FORCE)

set(RUNTIME_DEST bin)
set(LIBRARY_DEST lib/rush-${RUSH_VERSION_MAJOR})
set(INCLUDE_DEST include)

option(RUSH_BUILD_TESTS "Generate build targets for Rush unit tests." ON)
option(RUSH_BUILD_STATIC_LIBS "Generate build targets for static libraries." ON)
option(RUSH_BUILD_SHARED_LIBS "Generate build targets for shared libraries." OFF)

configure_file(include/rush/version.hpp.in include/rush/version.hpp)
project(rush VERSION ${RUSH_VERSION} LANGUAGES CXX)
list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_LIST_DIR}/cmake/modules")

include(RushAdd)
include(BuildType)
include(GNUInstallDirs)

# Clang does not respect LIBRARY_PATH in the way that it should
# forcing me to use an -L flag directly; not good, remove asap.
link_directories(/usr/local/lib)

find_package(LLVM REQUIRED CONFIG)

message(STATUS "Found LLVM ${LLVM_PACKAGE_VERSION}")
message(STATUS "Using LLVMConfig.cmake in: ${LLVM_DIR}")

include_directories(${LLVM_INCLUDE_DIRS})
add_definitions(${LLVM_DEFINITIONS})


if("${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang")
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -stdlib=libc++")
endif()


add_subdirectory(external)
add_subdirectory(utils)
add_subdirectory(lib)
add_subdirectory(tools)

install(DIRECTORY include/rush
   DESTINATION ${INCLUDE_DEST}
   FILES_MATCHING PATTERN "*.h??")

install(DIRECTORY ${CMAKE_BINARY_DIR}/include/rush
   DESTINATION ${INCLUDE_DEST}
   FILES_MATCHING PATTERN "*.h??")

install(TARGETS
   ast
   core
   diag
   lexer
   parser
   crush
   rupl
   RUNTIME DESTINATION ${RUNTIME_DEST}
   LIBRARY DESTINATION ${LIBRARY_DEST})

if(RUSH_BUILD_TESTS)
	include(Catch)
	enable_testing()
	add_subdirectory(test)
endif()
