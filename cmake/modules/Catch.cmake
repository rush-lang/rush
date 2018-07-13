# Distributed under the OSI-approved BSD 3-Clause License.  See accompanying
# file Copyright.txt or https://cmake.org/licensing for details.
#------------------------------------------------------------------------------
function(catch_discover_tests TARGET)
  cmake_parse_arguments(
    ""
    ""
    "TEST_PREFIX;TEST_SUFFIX;WORKING_DIRECTORY;TEST_LIST"
    "TEST_SPEC;EXTRA_ARGS;PROPERTIES"
    ${ARGN}
  )

  if(NOT _WORKING_DIRECTORY)
    set(_WORKING_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}")
  endif()
  if(NOT _TEST_LIST)
    set(_TEST_LIST ${TARGET}_TESTS)
  endif()

  ## Generate a unique name based on the extra arguments
  string(SHA1 args_hash "${_TEST_SPEC} ${_EXTRA_ARGS}")
  string(SUBSTRING ${args_hash} 0 7 args_hash)

  # Define rule to generate test list for aforementioned test executable
  set(ctest_include_file "${CMAKE_CURRENT_BINARY_DIR}/${TARGET}_include-${args_hash}.cmake")
  set(ctest_tests_file "${CMAKE_CURRENT_BINARY_DIR}/${TARGET}_tests-${args_hash}.cmake")
  get_property(crosscompiling_emulator
    TARGET ${TARGET}
    PROPERTY CROSSCOMPILING_EMULATOR
  )
  add_custom_command(
    TARGET ${TARGET} POST_BUILD
    BYPRODUCTS "${ctest_tests_file}"
    COMMAND "${CMAKE_COMMAND}"
            -D "TEST_TARGET=${TARGET}"
            -D "TEST_EXECUTABLE=$<TARGET_FILE:${TARGET}>"
            -D "TEST_EXECUTOR=${crosscompiling_emulator}"
            -D "TEST_WORKING_DIR=${_WORKING_DIRECTORY}"
            -D "TEST_SPEC=${_TEST_SPEC}"
            -D "TEST_EXTRA_ARGS=${_EXTRA_ARGS}"
            -D "TEST_PROPERTIES=${_PROPERTIES}"
            -D "TEST_PREFIX=${_TEST_PREFIX}"
            -D "TEST_SUFFIX=${_TEST_SUFFIX}"
            -D "TEST_LIST=${_TEST_LIST}"
            -D "CTEST_FILE=${ctest_tests_file}"
            -P "${_CATCH_DISCOVER_TESTS_SCRIPT}"
    VERBATIM
  )

  file(WRITE "${ctest_include_file}"
    "if(EXISTS \"${ctest_tests_file}\")\n"
    "  include(\"${ctest_tests_file}\")\n"
    "else()\n"
    "  add_test(${TARGET}_NOT_BUILT-${args_hash} ${TARGET}_NOT_BUILT-${args_hash})\n"
    "endif()\n"
  )

  if(NOT ${CMAKE_VERSION} VERSION_LESS "3.10.0")
    # Add discovered tests to directory TEST_INCLUDE_FILES
    set_property(DIRECTORY
      APPEND PROPERTY TEST_INCLUDE_FILES "${ctest_include_file}"
    )
  else()
    # Add discovered tests as directory TEST_INCLUDE_FILE if possible
    get_property(test_include_file_set DIRECTORY PROPERTY TEST_INCLUDE_FILE SET)
    if (NOT ${test_include_file_set})
      set_property(DIRECTORY
        PROPERTY TEST_INCLUDE_FILE "${ctest_include_file}"
      )
    else()
      message(FATAL_ERROR
        "Cannot set more than one TEST_INCLUDE_FILE"
      )
    endif()
  endif()

endfunction()

###############################################################################

set(_CATCH_DISCOVER_TESTS_SCRIPT
  ${CMAKE_CURRENT_LIST_DIR}/CatchAddTests.cmake
)

###############################################################################
