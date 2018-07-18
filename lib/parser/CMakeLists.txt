project(parser VERSION ${RUSH_VERSION} LANGUAGES CXX)

add_library(parser
	parser.cpp
)

target_include_directories(parser PUBLIC
	$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/../../include>
	$<INSTALL_INTERFACE:../../include>
)

target_link_libraries(parser PUBLIC lexer)

