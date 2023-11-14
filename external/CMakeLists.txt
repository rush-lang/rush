add_subdirectory(fmt)
add_subdirectory(tclr)
add_subdirectory(cxxopts)

execute_process(COMMAND git submodule update --init -- external/inja
                WORKING_DIRECTORY ${CMAKE_SOURCE_DIR})
add_subdirectory(inja)

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
