# =================================================================================================
# Add lizard when not building for WASM
# =================================================================================================
if(NOT CMAKE_BUILD_TYPE STREQUAL "Emscripten")
  list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}")
  include(FindLizard.cmake)
  find_package(Lizard)
endif()

# =================================================================================================
# argparse library
# =================================================================================================
block(SCOPE_FOR VARIABLES)
unset(CMAKE_CXX_CPPCHECK)
unset(CMAKE_C_CPPCHECK)

add_library(argparse_lib STATIC ./argparse/argparse.c)

target_include_directories(argparse_lib INTERFACE ./argparse/)

endblock()

# =================================================================================================
# cmock library
# =================================================================================================
block(SCOPE_FOR VARIABLES)
unset(CMAKE_CXX_CPPCHECK)
unset(CMAKE_C_CPPCHECK)

add_library(Unity STATIC ./unity/src/unity.c)
target_include_directories(Unity PUBLIC unity/src)

add_library(Cmock STATIC ./cmock/src/cmock.c)
target_include_directories(Cmock PUBLIC cmock/src)

target_link_libraries(Cmock Unity)

endblock()

# =================================================================================================
# Core libraries
# =================================================================================================
block(SCOPE_FOR VARIABLES)
unset(CMAKE_CXX_CPPCHECK)
unset(CMAKE_C_CPPCHECK)
add_subdirectory(core_lib/source/defs)
add_subdirectory(core_lib/misc/test_stubs)
add_subdirectory(core_lib/source/comp-itt_2_pp_buf_size)
add_subdirectory(core_lib/source/comp-itt_to_path)
add_subdirectory(core_lib/source/comp-sum_parity)
add_subdirectory(core_lib/source/comp-wptt_vertex_parity)
add_subdirectory(core_lib/source/note-wptt)
add_subdirectory(core_lib/source/note-plpath)
endblock()
