add_library(inja INTERFACE)

target_include_directories(inja INTERFACE
	$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
	$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/third_party>
	$<INSTALL_INTERFACE:include> # Not sure about this one.
)
