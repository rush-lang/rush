add_executable(core-test
	$<TARGET_OBJECTS:catch2>
	ascii_test.cpp
	ring_test.cpp
	iterator_test.cpp
	location_test.cpp
	source_manager_test.cpp
)

target_include_directories(core-test PRIVATE ../../../external/catch2/include)
target_link_libraries(core-test core)
catch_discover_tests(core-test)