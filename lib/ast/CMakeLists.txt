project(ast VERSION ${RUSH_VERSION} LANGUAGES CXX)

add_library(ast
	type.cpp
	literal.cpp
	binary.cpp)

target_compile_features(ast PUBLIC cxx_std_17)

target_include_directories(ast PUBLIC
	$<BUILD_INTERFACE:${CMAKE_CURRENT_BINARY_DIR}/../../include>
	$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/../../include>
	$<INSTALL_INTERFACE:../../include>
)