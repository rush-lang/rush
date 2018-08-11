add_executable(sema-test
	$<TARGET_OBJECTS:catch2>
	symbol_test.cpp
	scope_test.cpp
)

target_include_directories(sema-test PRIVATE ../../../external/catch2/include)
target_link_libraries(sema-test sema)
catch_discover_tests(sema-test)
