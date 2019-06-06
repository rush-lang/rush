project(parser VERSION ${RUSH_VERSION} LANGUAGES CXX)

rush_add_library(parser
   parse/statement.cpp
	parse/expression.cpp
   parse/declaration.cpp
   parse/module.cpp
   parse/type.cpp
	parse.cpp
   dump.cpp
   symbol.cpp
   scope.cpp
)

target_include_directories(parser PUBLIC
	$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/../../include>
	$<INSTALL_INTERFACE:../../include>
)

target_compile_features(parser PUBLIC cxx_std_17)

target_link_libraries(parser PUBLIC fmt lexer ast)

