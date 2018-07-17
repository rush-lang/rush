add_executable(rush-core-test
	$<TARGET_OBJECTS:catch2>
	charinfo_test.cpp
	iterator_test.cpp
	source_manager_test.cpp
)

target_include_directories(rush-core-test PRIVATE ../../../external/include)
target_link_libraries(rush-core-test rush-core)
catch_discover_tests(rush-core-test)