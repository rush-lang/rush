add_library(catch2 OBJECT catch.cpp)

target_include_directories(catch2 INTERFACE
	$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/..>
)