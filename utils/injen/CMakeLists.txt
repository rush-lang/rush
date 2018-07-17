cmake_minimum_required(VERSION 3.11)
project(injen VERSION 1.0.0 LANGUAGES CXX)

add_executable(injen
	src/main.cpp
)

target_link_libraries(injen PRIVATE inja)