execute_process(COMMAND git submodule update --init -- external/inja
                WORKING_DIRECTORY ${CMAKE_SOURCE_DIR})
add_subdirectory(inja)

execute_process(COMMAND git submodule update --init -- external/fmtlib
                WORKING_DIRECTORY ${CMAKE_SOURCE_DIR})
add_subdirectory(fmtlib)

if (RUSH_BUILD_TOOLS OR RUSH_BUILD_TESTS)
   execute_process(COMMAND git submodule update --init --recursive -- external/yaml-cpp
                   WORKING_DIRECTORY ${CMAKE_SOURCE_DIR})
   add_subdirectory(yaml-cpp)
endif()

if(RUSH_BUILD_TOOLS)
   execute_process(COMMAND git submodule update --init -- external/cxxopts
                   WORKING_DIRECTORY ${CMAKE_SOURCE_DIR})
   add_subdirectory(cxxopts)
   add_subdirectory(tclr)
endif()

if(RUSH_BUILD_TESTS AND NOT Catch2_FOUND)
   message(STATUS "Catch2 not found, using bundled version")
   execute_process(COMMAND git submodule update --init -- external/catch2
                   WORKING_DIRECTORY ${CMAKE_SOURCE_DIR})
   set(CMAKE_CXX_STANDARD_PREV ${CMAKE_CXX_STANDARD})
   set(CMAKE_CXX_STANDARD 17)
   add_subdirectory(catch2)
   set(CMAKE_CXX_STANDARD ${CMAKE_CXX_STANDARD_PREV})
   include(Catch)
endif()
