project(rush-lexer VERSION 0.1 LANGUAGES CXX)

add_library(rush-lexer
	lexer.cpp
	token.cpp
)

target_include_directories(rush-lexer PUBLIC
	$<BUILD_INTERFACE:${CMAKE_CURRENT_BINARY_DIR}/../../include>
	$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/../../include>
	$<INSTALL_INTERFACE:../../include>
)

