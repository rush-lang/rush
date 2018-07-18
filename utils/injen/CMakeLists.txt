cmake_minimum_required(VERSION 3.11)
project(injen VERSION ${RUSH_VERSION} LANGUAGES CXX)

add_executable(injen
	src/main.cpp
)

target_include_directories(injen PRIVATE include)
target_link_libraries(injen PRIVATE inja)