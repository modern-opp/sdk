cmake_minimum_required(VERSION 3.20.0)
project(opp_frontend)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(LLVM REQUIRED CONFIG)
find_package(GTest REQUIRED)

set(BISON_Parser_INPUT ${CMAKE_CURRENT_SOURCE_DIR}/src/parser/parser.ypp)
set(BISON_Parser_OUTPUT_SOURCE ${CMAKE_CURRENT_BINARY_DIR}/include/parser/parser.tab.cpp)
set(BISON_Parser_OUTPUT_HEADER ${CMAKE_CURRENT_BINARY_DIR}/include/parser/parser.tab.hpp)
file(MAKE_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/include/parser)
add_custom_command(
    OUTPUT ${BISON_Parser_OUTPUT_SOURCE} ${BISON_Parser_OUTPUT_HEADER}
    COMMAND bison
        --output=${BISON_Parser_OUTPUT_SOURCE}
        --defines=${BISON_Parser_OUTPUT_HEADER}
        ${BISON_Parser_INPUT}
    DEPENDS ${BISON_Parser_INPUT}
    COMMENT "Generating parser from ${BISON_Parser_INPUT}"
    WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
)

include_directories(${LLVM_INCLUDE_DIRS})
if(llvm_BUILD_DIRS_DEBUG)
    list(APPEND CMAKE_MODULE_PATH "${llvm_BUILD_DIRS_DEBUG}/llvm")
elseif(llvm_BUILD_DIRS_RELEASE)
    list(APPEND CMAKE_MODULE_PATH "${llvm_BUILD_DIRS_RELEASE}/llvm")
endif()
include(AddLLVM)
include_directories(${CMAKE_CURRENT_BINARY_DIR}/include)
include_directories("src/include")

set(SOURCES
        src/lexer/lexer.cpp
        src/driver.cpp
        src/ast/ast.cpp
        src/lexer/buffered_reader.cpp
        src/lexer/scanner.cpp
        src/util/token_utils.cpp
        src/visitor/pretty_print_visitor.cpp
        src/visitor/recursive_visitor.cpp
        src/include/ast/ast.hpp
        src/include/lexer/buffered_reader.hpp
        src/include/lexer/scanner.hpp
        src/include/util/token_utils.hpp
        src/include/visitor/pretty_print_visitor.hpp
        src/include/visitor/recursive_visitor.hpp
        ${BISON_Parser_OUTPUT_SOURCE}
)

add_executable(opp_frontend src/main.cpp ${SOURCES})

llvm_config(opp_frontend Analysis AsmParser DebugInfoDWARF Demangle IRReader Remarks ScalarOpts TextAPI TransformUtils)

enable_testing()

set(GOOD_TEST_DIR "${CMAKE_SOURCE_DIR}/test/opp-examples/good_examples/")
add_compile_definitions(PATH_GOOD_TEST="${GOOD_TEST_DIR}")
message(STATUS "GOOD TEST DIR ${GOOD_TEST_DIR}")

add_executable(
        lexer_test
        test/opp_tests.cpp
        ${SOURCES}
        src/lexer/scanner.cpp
)

target_link_libraries(
        lexer_test
        GTest::gtest_main
)

include(GoogleTest)
gtest_discover_tests(
        lexer_test
)
