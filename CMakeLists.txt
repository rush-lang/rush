cmake_minimum_required(VERSION 3.11)
set(RUSH_VERSION 1.0.0)

option(RUSH_BUILD_TESTS "Generate build targets for Rush unit tests." ON)
option(RUSH_BUILD_STATIC_LIBS "Generate build targets for static libraries." ON)
option(RUSH_BUILD_SHARED_LIBS "Generate build targets for shared libraries." OFF)

project(rush VERSION ${RUSH_VERSION} LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_LIST_DIR}/cmake/modules")

include(GNUInstallDirs)

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

if(RUSH_BUILD_TESTS)
	include(Catch)
	enable_testing()
	add_subdirectory(tests)
endif()