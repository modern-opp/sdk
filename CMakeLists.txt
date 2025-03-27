cmake_minimum_required(VERSION 3.23)

set(CMAKE_CXX_STANDARD 20)

project(opp_frontend)

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

bison_target(Parser src/parser/parser.ypp ${CMAKE_CURRENT_BINARY_DIR}/parser.tab.cpp)
include_directories(${CMAKE_CURRENT_BINARY_DIR})
include_directories("src/include")

add_executable(opp_frontend
        src/main.cpp
        src/lexer/lexer.cpp
        src/driver.cpp
        src/ast/ast.cpp
        src/include/ast.hpp
        ${BISON_Parser_OUTPUT_SOURCE}
)
