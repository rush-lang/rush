project(core VERSION ${RUSH_VERSION} LANGUAGES CXX)

add_library(core
	source_manager.cpp
)

target_include_directories(core PUBLIC
	$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/../../include>
	$<INSTALL_INTERFACE:../../include>
)

target_compile_features(core PUBLIC cxx_std_17)

llvm_map_components_to_libnames(llvm_libs support core irreader)
target_link_libraries(core PUBLIC ${llvm_libs})