add_executable(lexer-spike lexer.cpp)
target_link_libraries(lexer-spike PRIVATE lexer)
set_target_properties(lexer-spike PROPERTIES OUTPUT_NAME lexer)

add_executable(parser-spike parser.cpp)
target_link_libraries(parser-spike PRIVATE parser)
set_target_properties(parser-spike PROPERTIES OUTPUT_NAME parser )

add_executable(irgen-spike irgen.cpp)
target_link_libraries(irgen-spike PRIVATE parser sema)
set_target_properties(irgen-spike PROPERTIES OUTPUT_NAME irgen )
