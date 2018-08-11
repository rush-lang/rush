project(sema VERSION ${RUSH_VERSION} LANGUAGES CXX)

rush_add_library(sema
	scope.cpp
)

target_include_directories(sema PUBLIC
	$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/../../include>
	$<INSTALL_INTERFACE:../../include>
)

target_compile_features(sema PUBLIC cxx_std_17)
