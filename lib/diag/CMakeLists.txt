project(diag VERSION ${RUSH_VERSION} LANGUAGES CXX)

rush_add_library(diag
   diagnostic.cpp
)

target_include_directories(diag PUBLIC
	$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/../../include>
	$<INSTALL_INTERFACE:../../include>
)

target_compile_features(diag PUBLIC cxx_std_17)
target_link_libraries(diag PUBLIC lexer ast fmt)
