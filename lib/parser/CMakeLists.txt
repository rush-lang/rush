project(parser VERSION ${RUSH_VERSION} LANGUAGES CXX)

rush_add_library(parser
	parse_statement.cpp
	parse_expression.cpp
	parse_declaration.cpp
	parse.cpp
)

target_include_directories(parser PUBLIC
	$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/../../include>
	$<INSTALL_INTERFACE:../../include>
)

target_compile_features(parser PUBLIC cxx_std_17)

target_link_libraries(parser PUBLIC fmt lexer ast sema)

