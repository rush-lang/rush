cmake_minimum_required(VERSION 3.11)
project(crush VERSION ${RUSH_VERSION} LANGUAGES CXX)

add_executable(crush
	driver.cpp
)

target_include_directories(crush PRIVATE include)
target_link_libraries(crush PRIVATE cxxopts parser)

set_target_properties(crush PROPERTIES
	RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})
