cmake_minimum_required(VERSION 3.11)
project(rush VERSION 1.0.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_LIST_DIR}/cmake/modules")

include(GNUInstallDirs)

find_package(LLVM REQUIRED CONFIG)

message(STATUS "Found LLVM ${LLVM_PACKAGE_VERSION}")
message(STATUS "Using LLVMConfig.cmake in: ${LLVM_DIR}")

include_directories(${LLVM_INCLUDE_DIRS})
add_definitions(${LLVM_DEFINITIONS})


option(RUSH_BUILD_TESTS "Generate build targets for Rush unit tests." ON)
option(RUSH_BUILD_STATIC_LIBS "Generate build targets for static libraries." ON)
option(RUSH_BUILD_SHARED_LIBS "Generate build targets for shared libraries." OFF)


if("${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang")
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -stdlib=libc++")
endif()

# function(read_definition_file )
# read_definition_file(keywords.def RUSH_KEYWORDS)
# read_definition_file(types.def RUSH_BUILTIN_TYPES)

# # Read built-in types and store in a list
# file(READ cmake/templates/types.def RUSH_BUILTIN_TYPES)
# string(REGEX MATCHALL "(^|\n)[a-zA-Z_0-9]+" RUSH_BUILTIN_TYPES ${RUSH_BUILTIN_TYPES})
# string(REPLACE "\n" ";" RUSH_BUILTIN_TYPES ${RUSH_BUILTIN_TYPES})

# # Read keywords and store in a list
# file(READ cmake/templates/keywords.def RUSH_KEYWORDS)
# string(REGEX MATCHALL "(^|\n)[a-zA-Z_0-9]+" RUSH_KEYWORDS ${RUSH_KEYWORDS})
# string(REPLACE "\n" ";" RUSH_KEYWORDS ${RUSH_KEYWORDS})


# string(REPLACE ";" "_,\n\t\t" keywords "${RUSH_KEYWORDS}")
# configure_file(
# 	${CMAKE_CURRENT_SOURCE_DIR}/include/rush/lexer/generated-keywords.hpp.in
# 	${CMAKE_CURRENT_SOURCE_DIR}/include/rush/lexer/keywords.hpp)


add_subdirectory(external)
add_subdirectory(utils)
add_subdirectory(lib)
add_subdirectory(tools)

if(RUSH_BUILD_TESTS)
	include(Catch)
	enable_testing()
	add_subdirectory(tests)
endif()