add_executable(parser-test
	parser_test.cpp
   result_test.cpp
)

# target_include_directories(parser-test PRIVATE ../../../external/catch2/include)
target_link_libraries(parser-test PRIVATE Catch2::Catch2WithMain parser)
catch_discover_tests(parser-test)
