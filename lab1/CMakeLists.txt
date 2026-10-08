cmake_minimum_required(VERSION 3.20)
project(lab1 CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Debug)
endif()

enable_testing()

# ---------- Set ----------
add_library(set_lib
    set/src/Element.cpp
    set/src/Set.cpp
    set/src/SetParser.cpp
)
target_include_directories(set_lib PUBLIC set/include)

add_executable(set_cli set/cli/main_set.cpp)
target_link_libraries(set_cli PRIVATE set_lib)

# ---------- Turing ----------
add_library(turing_lib
    turing/src/Alphabet.cpp
    turing/src/Tape.cpp
    turing/src/Head.cpp
    turing/src/Rule.cpp
    turing/src/Program.cpp
    turing/src/TuringMachine.cpp
)
target_include_directories(turing_lib PUBLIC turing/include)

add_executable(turing_cli turing/cli/main_turing.cpp)
target_link_libraries(turing_cli PRIVATE turing_lib)

# ---------- Tests ----------
include(FetchContent)

FetchContent_Declare(
  googletest
  GIT_REPOSITORY https://github.com/google/googletest.git
  GIT_TAG        f8d7d77c06936315286eb55f8de22cd23c188571  # v1.14.0
)
FetchContent_MakeAvailable(googletest)

add_executable(set_tests
    set/tests/test_element.cpp
    set/tests/test_set.cpp
    set/tests/test_parser.cpp
)
target_link_libraries(set_tests PRIVATE set_lib GTest::gtest_main)

add_executable(turing_tests
    turing/tests/test_tape.cpp
    turing/tests/test_program.cpp
    turing/tests/test_machine.cpp
)
target_link_libraries(turing_tests PRIVATE turing_lib GTest::gtest_main)

add_test(NAME set_tests COMMAND set_tests)
add_test(NAME turing_tests COMMAND turing_tests)

# ---------- Coverage ----------
if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    target_compile_options(set_lib PRIVATE --coverage -O0 -g)
    target_link_options(set_lib PUBLIC --coverage)      # пробрасывается в set_cli и set_tests

    target_compile_options(turing_lib PRIVATE --coverage -O0 -g)
    target_link_options(turing_lib PUBLIC --coverage)   # пробрасывается в turing_cli и turing_tests
endif()
