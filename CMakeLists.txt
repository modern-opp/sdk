cmake_minimum_required(VERSION 3.20.0)
project(opp_frontend)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(LLVM REQUIRED CONFIG)
find_package(GTest REQUIRED)

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

set(BISON_Parser_INPUT ${CMAKE_CURRENT_SOURCE_DIR}/src/parser/parser.ypp)
set(BISON_Parser_OUTPUT_SOURCE ${CMAKE_CURRENT_BINARY_DIR}/include/parser/parser.tab.cpp)
set(BISON_Parser_OUTPUT_HEADER ${CMAKE_CURRENT_BINARY_DIR}/include/parser/parser.tab.hpp)
file(MAKE_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/include/parser)
add_custom_command(
    OUTPUT ${BISON_Parser_OUTPUT_SOURCE} ${BISON_Parser_OUTPUT_HEADER}
    COMMAND ${BISON_EXECUTABLE}
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
        src/semantic/mangling_transformer.cpp
        src/semantic/symbol.cpp
        src/semantic/symbol_table.cpp
        src/semantic/symbol_table_index.cpp
        src/semantic/symbol_table_class_collector_visitor.cpp
        src/semantic/symbol_table_method_collector_visitor.cpp
        src/semantic/symbol_table_visitor.cpp
        src/semantic/cfa_visitor.cpp
#        src/semantic/type_checker_visitor.cpp
        src/lexer/buffered_reader.cpp
        src/lexer/scanner.cpp
        src/util/token_utils.cpp
        src/visitor/pretty_print_visitor.cpp
        src/stdlib/builtins.cpp
        src/include/ast/ast.hpp
        src/include/lexer/buffered_reader.hpp
        src/include/lexer/scanner.hpp
        src/include/semantic/mangling_transformer.hpp
        src/include/semantic/symbol.hpp
        src/include/semantic/entrypoint_visitor.hpp
#        src/include/semantic/type_checker_visitor.hpp
        src/include/semantic/symbol_table.hpp
        src/include/semantic/symbol_table_index.hpp
        src/include/semantic/symbol_table_class_collector_visitor.hpp
        src/include/semantic/symbol_table_visitor.hpp
        src/include/semantic/symbol_table_method_collector_visitor.hpp
        src/include/semantic/semantic_error.hpp
        src/include/semantic/cfa_visitor.hpp
        src/include/stdlib/builtins.hpp
        src/include/util/token_utils.hpp
        src/include/visitor/pretty_print_visitor.hpp
        src/include/visitor/recursive_visitor.hpp
        src/include/visitor/simple_visitor.hpp
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
)

target_link_libraries(
        lexer_test
        GTest::gtest_main
)

include(GoogleTest)
gtest_discover_tests(
        lexer_test
)

get_cmake_property(_variableNames VARIABLES)
list (SORT _variableNames)
foreach (_variableName ${_variableNames})
    message(STATUS "${_variableName}=${${_variableName}}")
endforeach()
