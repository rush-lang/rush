add_executable(parser-test
	$<TARGET_OBJECTS:catch2>
	parser_test.cpp
   result_test.cpp
)

target_include_directories(parser-test PRIVATE ../../../external/catch2/include)
target_link_libraries(parser-test parser)
catch_discover_tests(parser-test)
