project(rush-parser VERSION 0.1 LANGUAGES CXX)

add_library(rush-parser
	parser.cpp
)

target_include_directories(rush-parser PUBLIC
	$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/../../include>
	$<INSTALL_INTERFACE:../../include>
)

