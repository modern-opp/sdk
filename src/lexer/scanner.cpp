//
// Created by Nikita Morozov on 27.03.2025.
//

#include "lexer/scanner.hpp"

#include <utility>
#include "lexer/buffered_reader.hpp"

// BEGIN PREDICATES BLOCK

static constexpr bool is_special(char peek) noexcept {
    return peek == '.' || peek == '(' || peek == ')'
           || peek == ':' || peek == ']' || peek == '['
           || peek == ',' || peek == '=' || peek == '>';
}

static constexpr bool is_whitespace(char peek) noexcept {
    return peek == ' ' || peek == '\t' || peek == '\r' || peek == '\n';
}

static constexpr bool is_spliterator(char peek) {
    return is_whitespace(peek) || is_special(peek);
}

static constexpr bool is_num_spliterator(char peek) {
    return is_whitespace(peek)
           || peek == '(' || peek == ')' || peek == ']' || peek == '['
           || peek == ',' || peek == '=' || peek == '>' || peek == ':';
}

// END PREDICATES BLOCK


yy::parser::symbol_type yy::Scanner::get_literal_identifier_or_keyword() {
    begin_token();
    std::string s;

    while (!reader_.eof() && !is_spliterator(peek())) {
        s += advance();
    }

    if (s == "true")
        return yy::parser::make_BOOLEAN_LITERAL(true, end_token());
    if (s == "false")
        return yy::parser::make_BOOLEAN_LITERAL(false, end_token());
    if (s == "class")
        return yy::parser::make_CLASS(end_token());
    if (s == "extends")
        return yy::parser::make_EXTENDS(end_token());
    if (s == "is")
        return yy::parser::make_IS(end_token());
    if (s == "end")
        return yy::parser::make_END(end_token());
    if (s == "this")
        return yy::parser::make_THIS(end_token());
    if (s == "method")
        return yy::parser::make_METHOD(end_token());
    if (s == "return")
        return yy::parser::make_RETURN(end_token());
    if (s == "var")
        return yy::parser::make_VAR(end_token());
    if (s == "if")
        return yy::parser::make_IF(end_token());
    if (s == "then")
        return yy::parser::make_THEN(end_token());
    if (s == "else")
        return yy::parser::make_ELSE(end_token());
    if (s == "while")
        return yy::parser::make_WHILE(end_token());
    if (s == "loop")
        return yy::parser::make_LOOP(end_token());

    return yy::parser::make_IDENTIFIER(s, end_token());
}

yy::parser::symbol_type yy::Scanner::get_string_literal() {
    begin_token();
    advance();

    std::string s;
    while (!reader_.eof() && peek() != '\'') {
        s += advance();
    }

    if (reader_.eof()) {
        return yy::parser::make_YYUNDEF(end_token());
    }
    advance();
    return yy::parser::make_STRING_LITERAL(s, end_token());
}

yy::parser::symbol_type yy::Scanner::get_num_literal() {
    begin_token();
    size_t dots = 0;

    std::string s;
    while (!reader_.eof() && !is_num_spliterator(peek())) {
        if (peek() == '.') {
            dots++;
        }
        s += advance();
    }

    if (dots > 1) {
        return yy::parser::make_YYUNDEF(end_token());
    }

    if (dots == 1) {
        return yy::parser::make_REAL_LITERAL(std::stod(s), end_token());
    }

    return yy::parser::make_INTEGER_LITERAL(std::stoi(s), end_token());
}


yy::parser::symbol_type yy::Scanner::get_special() {
    begin_token();
    std::string s;

    while (s.length() < 2) {
        s += advance();

        if (s == "(")
            return yy::parser::make_LEFT_PAREN(end_token());
        if (s == ")")
            return yy::parser::make_RIGHT_PAREN(end_token());
        if (s == "]")
            // TODO: implement "]" token
            return yy::parser::make_LEFT_PAREN(end_token());
        if (s == "[")
            // TODO: implement "[" token
            return yy::parser::make_RIGHT_PAREN(end_token());
        if (s == ":")
            return yy::parser::make_COLON(end_token());
        if (s == ",")
            return yy::parser::make_COMMA(end_token());
        if (s == "=>")
            return yy::parser::make_METHOD_DEFINITION(end_token());
        if (s == ":=")
            return yy::parser::make_ASSIGNMENT_OPERATOR(end_token());
        if (s == ".")
            return yy::parser::make_MEMBER_ACCESS_OPERATOR(end_token());
    }

    return yy::parser::make_YYUNDEF(end_token());
}

yy::parser::symbol_type yy::Scanner::get_identifier_or_undef() {
    begin_token();
    std::string s;

    while (!reader_.eof() && !is_spliterator(peek())) {
        s += advance();
    }

    return yy::parser::make_IDENTIFIER(s, end_token());
}


yy::Scanner::Scanner(
        yy::BufferedReader reader,
        const std::string &filename
) : filename_(filename), reader_(std::move(reader))
     {}

char yy::Scanner::peek() const {
    return reader_.peek();
}

char yy::Scanner::advance() {
    char ch = reader_.peek();
    if (ch == '\n') {
        lines_ += 1;
        lineOffset_ = 0;
    } else {
        lineOffset_++;
    }

    return reader_.advance();
}

yy::parser::symbol_type yy::Scanner::get_token() {
    begin_token();
    while (!reader_.eof()) {
        char next = peek();

        while (is_whitespace(next)) {
            advance();

            if (reader_.eof()) {
                return parser::make_YYEOF(end_token());
            }

            next = peek();
        }


        if ('a' <= next && next <= 'z') {
            return get_literal_identifier_or_keyword();
        }

        if (next == '\'') {
            return get_string_literal();
        }

        if ('0' <= next && next <= '9') {
            return get_num_literal();
        }

        if (is_special(next)) {
            return get_special();
        }

        return get_identifier_or_undef();
    }

    return parser::make_YYEOF(end_token());
}

void yy::Scanner::begin_token() {
    begin_ = yy::position(&filename_, lines_, lineOffset_);
}

yy::location yy::Scanner::end_token() {
    yy::position end{&filename_, lines_, lineOffset_};
    return {{begin_}, end};
}
