add_executable(lexer-test
	$<TARGET_OBJECTS:catch2>
	lexer_test.cpp
	token_test.cpp
)

target_include_directories(lexer-test PRIVATE ../../../external/catch2/include)
target_link_libraries(lexer-test lexer)
catch_discover_tests(lexer-test)
