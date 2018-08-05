add_library(json INTERFACE)

target_include_directories(json INTERFACE
	$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
	$<INSTALL_INTERFACE:include> # Not sure about this one.
)
