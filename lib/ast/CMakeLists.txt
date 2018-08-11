project(ast VERSION ${RUSH_VERSION} LANGUAGES CXX)

rush_add_library(ast
	expr/literal.cpp
	expr/binary.cpp
	expr/unary.cpp
	type.cpp
)

target_compile_features(ast PUBLIC cxx_std_17)


rush_render_templates(
	${CMAKE_SOURCE_DIR}/include/rush/ast/_operators.hpp.j2
	${CMAKE_SOURCE_DIR}/include/rush/ast/expr/_expressions.hpp.j2
	JSON ${CMAKE_SOURCE_DIR}/include/rush/ast/operators.json
	DEPENDENTS ast
)

target_include_directories(ast PUBLIC
	$<BUILD_INTERFACE:${CMAKE_CURRENT_BINARY_DIR}/../../include>
	$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/../../include>
	$<INSTALL_INTERFACE:../../include>
)

target_link_libraries(ast PUBLIC sema)