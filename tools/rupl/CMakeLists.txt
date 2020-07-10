cmake_minimum_required(VERSION 3.11)
project(rupl VERSION ${RUSH_VERSION} LANGUAGES CXX)

add_executable(rupl
   driver.cpp
)

target_include_directories(rupl PRIVATE include)
target_link_libraries(rupl PRIVATE tclr cxxopts lexer parser)

set_target_properties(rupl PROPERTIES
	RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})
