

if (RUSH_BUILD_TESTS)
	add_subdirectory(include/catch2)
endif()

add_subdirectory(include/inja)
add_subdirectory(include/cxxopts)
add_subdirectory(include/nlohmann)