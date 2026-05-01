if(NOT CMAKE_BUILD_TYPE STREQUAL "Emscripten")
  add_executable(itt2plp main.cpp)

  target_link_libraries(itt2plp argparse_lib comp_i2pp_buf_size
                        comp_itt_to_path notation_wptt notation_plpath)

  target_include_directories(itt2plp PUBLIC .)
  file(GLOB_RECURSE test_files "${CMAKE_CURRENT_SOURCE_DIR}/test_*.*")

  pytest_discover_tests(
    PythonTest
    WORKING_DIRECTORY
    ${CMAKE_BINARY_DIR}
    TEST_PATHS
    ${CMAKE_CURRENT_SOURCE_DIR}
    DEPENDS
    itt2plp
    ${test_files}
    DISCOVERY_EXTRA_ARGS
    "--capture=no")

  file(COPY ${test_files} DESTINATION ${CMAKE_BINARY_DIR})

endif(NOT CMAKE_BUILD_TYPE STREQUAL "Emscripten")
