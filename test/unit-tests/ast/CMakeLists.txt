add_executable(ast-test
	expressions_test.cpp
	declarations_test.cpp
)

target_link_libraries(ast-test PRIVATE Catch2::Catch2WithMain ast)
catch_discover_tests(ast-test)
