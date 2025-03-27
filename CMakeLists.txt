cmake_minimum_required(VERSION 3.23)
project(opp_frontend)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

if (CMAKE_HOST_SYSTEM_NAME MATCHES "Darwin")
    execute_process(
            COMMAND brew --prefix bison
            RESULT_VARIABLE BREW_BISON
            OUTPUT_VARIABLE BREW_BISON_PREFIX
            OUTPUT_STRIP_TRAILING_WHITESPACE
    )
    if (BREW_BISON EQUAL 0 AND EXISTS "${BREW_BISON_PREFIX}")
        message(STATUS "Found Bison keg installed by Homebrew at ${BREW_BISON_PREFIX}")
        set(BISON_EXECUTABLE "${BREW_BISON_PREFIX}/bin/bison")
    endif ()
endif ()

find_package(BISON REQUIRED)

include(FetchContent)
FetchContent_Declare(
        googletest
        URL https://github.com/google/googletest/archive/03597a01ee50ed33e9dfd640b249b4be3799d395.zip
)
FetchContent_MakeAvailable(googletest)

bison_target(Parser src/parser/parser.ypp ${CMAKE_CURRENT_BINARY_DIR}/parser.tab.cpp)
include_directories(${CMAKE_CURRENT_BINARY_DIR})
include_directories("src/include")

set(SOURCES
        src/lexer/lexer.cpp
        src/driver.cpp
        src/ast/ast.cpp
        src/include/ast.hpp
        ${BISON_Parser_OUTPUT_SOURCE}
)

add_executable(opp_frontend src/main.cpp ${SOURCES})

enable_testing()


set(GOOD_TEST_DIR "${CMAKE_SOURCE_DIR}/test/opp-examples/good_examples/")
add_compile_definitions(PATH_GOOD_TEST="${GOOD_TEST_DIR}")
message(STATUS "GOOD TEST DIR ${GOOD_TEST_DIR}")

add_executable(
        lexer_test
        test/opp_tests.cpp
        ${SOURCES}
)
target_link_libraries(
        lexer_test
        GTest::gtest_main
)

include(GoogleTest)
gtest_discover_tests(
        lexer_test
)