add_library(ast INTERFACE)

target_compile_features(ast INTERFACE cxx_std_17)

target_include_directories(ast INTERFACE
	$<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}/include
	$<INSTALL_INTERFACE:../../include)