add_executable(parser-test
   driver.cpp
)

file(COPY fixtures DESTINATION ${CMAKE_CURRENT_BINARY_DIR})

target_link_libraries(parser-test PRIVATE
   Catch2::Catch2WithMain
   yaml-cpp::yaml-cpp
   parser)
catch_discover_tests(parser-test)
