add_executable(lexer-test
	lexer_test.cpp
	token_test.cpp
	lookahead_test.cpp
)

# target_include_directories(lexer-test PRIVATE ../../../external/catch2/include)
target_link_libraries(lexer-test PRIVATE Catch2::Catch2WithMain lexer)
catch_discover_tests(lexer-test)
