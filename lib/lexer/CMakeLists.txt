project(lexer VERSION ${RUSH_VERSION} LANGUAGES CXX)

add_library(lexer
	lexer.cpp
	token.cpp
)

target_include_directories(lexer PUBLIC
	$<BUILD_INTERFACE:${CMAKE_CURRENT_BINARY_DIR}/../../include>
	$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/../../include>
	$<INSTALL_INTERFACE:../../include>
)
