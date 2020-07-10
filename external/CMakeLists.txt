

if (RUSH_BUILD_TESTS)
	add_subdirectory(catch2)
endif()

add_subdirectory(fmt)
add_subdirectory(tclr)
add_subdirectory(inja)
add_subdirectory(cxxopts)
