add_executable(core-test
	ascii_test.cpp
	ring_test.cpp
	iterator_test.cpp
	location_test.cpp
	source_manager_test.cpp
)

target_link_libraries(core-test PRIVATE Catch2::Catch2WithMain core)
catch_discover_tests(core-test)
