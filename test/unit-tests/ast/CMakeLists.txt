add_executable(ast-test
	$<TARGET_OBJECTS:catch2>
	expressions_test.cpp
	declarations_test.cpp
)

target_include_directories(ast-test PRIVATE ../../../external/catch2/include)
target_link_libraries(ast-test ast)
catch_discover_tests(ast-test)