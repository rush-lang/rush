add_executable(rush-parser-test
	$<TARGET_OBJECTS:catch2>
	parser_test.cpp
)

target_include_directories(rush-parser-test PRIVATE ../../../external/catch2)
target_link_libraries(rush-parser-test rush-parser)
catch_discover_tests(rush-parser-test)
