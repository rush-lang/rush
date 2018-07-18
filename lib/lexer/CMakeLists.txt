project(lexer VERSION ${RUSH_VERSION} LANGUAGES CXX)

add_library(lexer
	lexer.cpp
	token.cpp
)

rush_render_templates(
	${CMAKE_SOURCE_DIR}/include/rush/lexer/_tokens.hpp.j2
	${CMAKE_SOURCE_DIR}/include/rush/lexer/_symbols.hpp.j2
	${CMAKE_SOURCE_DIR}/include/rush/lexer/_keywords.hpp.j2
	JSON ${CMAKE_SOURCE_DIR}/include/rush/lexer/tokens.json
	DEPENDENTS lexer
)

target_compile_features(lexer PUBLIC cxx_std_17)

target_include_directories(lexer PUBLIC
	$<BUILD_INTERFACE:${CMAKE_CURRENT_BINARY_DIR}/../../include>
	$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/../../include>
	$<INSTALL_INTERFACE:../../include>
)
