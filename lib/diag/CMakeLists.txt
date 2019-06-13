project(diag VERSION ${RUSH_VERSION} LANGUAGES CXX)

rush_add_library(diag
   diagnostic.cpp
   dump.cpp
)

target_include_directories(diag PUBLIC
	$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/../../include>
	$<INSTALL_INTERFACE:../../include>
)

rush_render_templates(
	${CMAKE_SOURCE_DIR}/include/rush/diag/_diagnostics.hpp.j2
	JSON ${CMAKE_SOURCE_DIR}/include/rush/diag/diagnostics.json
	DEPENDENTS diag
)

target_compile_features(diag PUBLIC cxx_std_17)
target_link_libraries(diag PUBLIC fmt lexer ast)
