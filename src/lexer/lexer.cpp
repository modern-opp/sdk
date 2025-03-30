#include<string>
#include<iostream>
#include "parser.tab.hpp"
#include "token_utils.hpp"
#include "scanner.hpp"


namespace yy {

    constexpr bool is_special(char peek) noexcept {
        return peek == '.' || peek == '(' || peek == ')'
               || peek == ':' || peek == ']' || peek == '['
               || peek == ',' || peek == '=' || peek == '>';
    }

    constexpr bool is_whitespace(char peek) noexcept {
        return peek == ' ' || peek == '\t' || peek == '\r' || peek == '\n';
    }

    bool is_spliterator(char peek) {
        return is_whitespace(peek) || is_special(peek);
    }

    bool is_num_spliterator(char peek) {
        return is_whitespace(peek)
               || peek == '(' || peek == ')' || peek == ']' || peek == '['
               || peek == ',' || peek == '=' || peek == '>' || peek == ':';
    }

    parser::symbol_type get_literal_identifier_or_keyword(Scanner &scanner) {
        scanner.begin_token();
        std::string s;

        while (!scanner.eof() && !is_spliterator(scanner.peek())) {
            s += scanner.advance();
        }

        if (s == "true")
            return yy::parser::make_BOOLEAN_LITERAL(true, scanner.end_token());
        if (s == "false")
            return yy::parser::make_BOOLEAN_LITERAL(false, scanner.end_token());
        if (s == "class")
            return yy::parser::make_CLASS(scanner.end_token());
        if (s == "extends")
            return yy::parser::make_EXTENDS(scanner.end_token());
        if (s == "is")
            return yy::parser::make_IS(scanner.end_token());
        if (s == "end")
            return yy::parser::make_END(scanner.end_token());
        if (s == "this")
            return yy::parser::make_THIS(scanner.end_token());
        if (s == "method")
            return yy::parser::make_METHOD(scanner.end_token());
        if (s == "return")
            return yy::parser::make_RETURN(scanner.end_token());
        if (s == "var")
            return yy::parser::make_VAR(scanner.end_token());
        if (s == "if")
            return yy::parser::make_IF(scanner.end_token());
        if (s == "then")
            return yy::parser::make_THEN(scanner.end_token());
        if (s == "else")
            return yy::parser::make_ELSE(scanner.end_token());
        if (s == "while")
            return yy::parser::make_WHILE(scanner.end_token());
        if (s == "loop")
            return yy::parser::make_LOOP(scanner.end_token());

        return yy::parser::make_IDENTIFIER(s, scanner.end_token());
    }

    parser::symbol_type get_string_literal(Scanner &scanner) {
        scanner.begin_token();
        scanner.advance();

        std::string s;
        while (!scanner.eof() && scanner.peek() != '\'') {
            s += scanner.advance();
        }

        if (scanner.eof()) {
            return yy::parser::make_YYUNDEF(scanner.end_token());
        }
        scanner.advance();
        return yy::parser::make_STRING_LITERAL(s, scanner.end_token());
    }

    parser::symbol_type get_num_literal(Scanner &scanner) {
        scanner.begin_token();
        size_t dots = 0;

        std::string s;
        while (!scanner.eof() && !is_num_spliterator(scanner.peek())) {
            if (scanner.peek() == '.') {
                dots++;
            }
            s += scanner.advance();
        }

        if (dots > 1) {
            return yy::parser::make_YYUNDEF(scanner.end_token());
        }

        if (dots == 1) {
            return yy::parser::make_REAL_LITERAL(std::stod(s), scanner.end_token());
        }

        return yy::parser::make_INTEGER_LITERAL(std::stoi(s), scanner.end_token());
    }


    parser::symbol_type get_special(Scanner &scanner) {
        scanner.begin_token();
        std::string s;

        while (s.length() < 2) {
            s += scanner.advance();

            if (s == "(")
                return yy::parser::make_LEFT_PAREN(scanner.end_token());
            if (s == ")")
                return yy::parser::make_RIGHT_PAREN(scanner.end_token());
            if (s == "]")
                return yy::parser::make_LEFT_PAREN(scanner.end_token());
            if (s == "[")
                return yy::parser::make_RIGHT_PAREN(scanner.end_token());
            if (s == ":")
                return yy::parser::make_COLON(scanner.end_token());
            if (s == ",")
                return yy::parser::make_COMMA(scanner.end_token());
            if (s == "=>")
                return yy::parser::make_METHOD_DEFINITION(scanner.end_token());
            if (s == ":=")
                return yy::parser::make_ASSIGNMENT_OPERATOR(scanner.end_token());
            if (s == ".")
                return yy::parser::make_MEMBER_ACCESS_OPERATOR(scanner.end_token());
        }

        return yy::parser::make_YYUNDEF(scanner.end_token());
    }

    parser::symbol_type get_identifier_or_undef(Scanner &scanner) {
        scanner.begin_token();
        std::string s;

        while (!scanner.eof() && !is_spliterator(scanner.peek())) {
            s += scanner.advance();
        }

        return yy::parser::make_IDENTIFIER(s, scanner.end_token());
    }

    parser::symbol_type get_token(Scanner &scanner) {
        scanner.begin_token();
        while (!scanner.eof()) {
            char next = scanner.peek();

            while (is_whitespace(next)) {
                scanner.advance();

                if (scanner.eof()) {
                    return parser::make_YYEOF(scanner.end_token());
                }

                next = scanner.peek();
            }


            if ('a' <= next && next <= 'z') {
                return get_literal_identifier_or_keyword(scanner);
            }

            if (next == '\'') {
                return get_string_literal(scanner);
            }

            if ('0' <= next && next <= '9') {
                return get_num_literal(scanner);
            }

            if (is_special(next)) {
                return get_special(scanner);
            }

            return get_identifier_or_undef(scanner);
        }

        return parser::make_YYEOF(scanner.end_token());
    }

    parser::symbol_type yylex(Scanner &scanner) {
        parser::symbol_type &&token = get_token(scanner);
        std::cout << token_to_string(token.kind()) << std::endl;

        return std::forward<parser::symbol_type>(token);
    }


}