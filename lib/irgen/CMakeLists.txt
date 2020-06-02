project(irgen VERSION ${RUSH_VERSION} LANGUAGES CXX)

rush_add_library(irgen llvm.cpp)

target_include_directories(irgen PUBLIC
	$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/../../include>
	$<INSTALL_INTERFACE:../../include>
)

target_compile_features(irgen PUBLIC cxx_std_17)

target_link_libraries(irgen PUBLIC diag parser lexer ast)
