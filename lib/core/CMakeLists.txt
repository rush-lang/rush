project(rush-core VERSION 0.1 LANGUAGES CXX)

add_library(rush-core
	source_manager.cpp
)

target_include_directories(rush-core PUBLIC
	$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/../../include>
	$<INSTALL_INTERFACE:../../include>
)

llvm_map_components_to_libnames(llvm_libs support core irreader)
target_link_libraries(rush-core PUBLIC ${llvm_libs})