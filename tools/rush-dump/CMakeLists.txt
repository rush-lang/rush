cmake_minimum_required(VERSION 3.11)
project(rush-dump VERSION ${RUSH_VERSION} LANGUAGES CXX)

add_executable(rush-dump
	driver.cpp
)

target_include_directories(rush-dump PRIVATE include)
target_link_libraries(rush-dump PRIVATE cxxopts parser)

set_target_properties(rush-dump PROPERTIES
	RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})