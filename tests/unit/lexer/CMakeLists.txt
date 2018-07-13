add_executable(rush-lexer-test
	$<TARGET_OBJECTS:catch2>
	lexer_test.cpp
	token_test.cpp
)

target_include_directories(rush-lexer-test PRIVATE ../../../external/catch2)
target_link_libraries(rush-lexer-test rush-lexer)
catch_discover_tests(rush-lexer-test)
