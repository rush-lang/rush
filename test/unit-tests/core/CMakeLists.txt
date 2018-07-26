add_executable(core-test
	$<TARGET_OBJECTS:catch2>
	charinfo_test.cpp
	iterator_test.cpp
	location_test.cpp
	source_manager_test.cpp
)

target_include_directories(core-test PRIVATE ../../../external/include)
target_link_libraries(core-test core)
catch_discover_tests(core-test)