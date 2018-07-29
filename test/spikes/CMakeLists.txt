add_executable(spike main.cpp)
target_link_libraries(spike PRIVATE core ast lexer parser)