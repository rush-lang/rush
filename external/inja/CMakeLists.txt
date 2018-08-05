add_library(inja INTERFACE)

target_include_directories(inja INTERFACE
	$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
	$<INSTALL_INTERFACE:include> # Not sure about this one.
)

target_link_libraries(inja INTERFACE json)
