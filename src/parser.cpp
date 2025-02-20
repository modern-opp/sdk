#include<iostream>
#include<fstream>
#include<string>
#include "parser.tab.hpp"
#include "driver.hpp"


namespace yy {


    enum TokenContext {
        free,
        id,
        special,
        integer,
        real,
        string
    };

    parser::symbol_type get_token(std::string s, TokenContext context, const location &l) {
        if (s == "true")
            return yy::parser::make_BOOLEAN_LITERAL(true, l);
        if (s == "false")
            return yy::parser::make_BOOLEAN_LITERAL(false, l);
        if (s == "class")
            return yy::parser::make_CLASS(l);
        if (s == "extends")
            return yy::parser::make_EXTENDS(l);
        if (s == "is")
            return yy::parser::make_IS(l);
        if (s == "end")
            return yy::parser::make_END(l);
        if (s == "this")
            return yy::parser::make_THIS(l);
        if (s == "method")
            return yy::parser::make_METHOD(l);
        if (s == "(")
            return yy::parser::make_LEFT_PAREN(l);
        if (s == ")")
            return yy::parser::make_RIGHT_PAREN(l);
        if (s == ":")
            return yy::parser::make_COLON(l);
        if (s == ",")
            return yy::parser::make_COMMA(l);
        if (s == "=>")
            return yy::parser::make_METHOD_DEFINITION(l);
        if (s == "return")
            return yy::parser::make_RETURN(l);
        if (s == "var")
            return yy::parser::make_VAR(l);
        if (s == ":=")
            return yy::parser::make_ASSIGNMENT_OPERATOR(l);
        if (s == ".")
            return yy::parser::make_MEMBER_ACCESS_OPERATOR(l);
        if (s == "if")
            return yy::parser::make_IF(l);
        if (s == "then")
            return yy::parser::make_THEN(l);
        if (s == "else")
            return yy::parser::make_ELSE(l);
        if (s == "while")
            return yy::parser::make_WHILE(l);
        if (s == "loop")
            return yy::parser::make_LOOP(l);
        if (!s.empty() && context == id)
            return yy::parser::make_IDENTIFIER(s, l);
        if (!s.empty() && context == integer)
            return yy::parser::make_INTEGER_LITERAL(std::stoi(s), l);
        if (!s.empty() && context == real)
            return yy::parser::make_REAL_LITERAL(std::stod(s), l);
        if (!s.empty() && context == string)
            return yy::parser::make_STRING_LITERAL(s, l);

        return yy::parser::make_YYUNDEF(l);
    }

    parser::symbol_type yylex(driver &driver) {
        auto &fin = driver.fin;
        TokenContext context = free;
        std::string s;
        yy::position begin = driver.location.begin, end;
        char c;
        while (true) {
            const location &l = location(begin, end);
            driver.location = l;

            if (!fin.eof())
                fin.get(c);
            else
                return yy::parser::make_YYEOF(l);

            if (c == ' ' || c == '\n' || fin.eof()) {
                if (c == ' ') {
                    end.columns(1);
                } else {
                    end.lines(1);
                }


                if (s.empty()) {
                    if (fin.eof()) {
                        return yy::parser::make_YYEOF(l);
                    }
                    context = free;
                    continue;
                } else {
                    if (!fin.eof()) {
                        fin.unget();
                    }

                    std::cerr << s << std::endl;

                    return get_token(s, context, l);
                }
            } else if ((c == '-' || (c >= '0' && c <= '9'))
                       && (context == free || context == integer || context == real)) {
                context = integer;
                s += c;
            } else if (c == 'e' && (context == real || context == integer)) {
                context = real;
                s += c;
            } else if (c == '.') {
                s += c;

                if (context == integer) {
                    context = real;
                    continue;
                }

                if (context != free) {
                    if (!fin.eof()) {
                        fin.unget();
                    }

                    std::cerr << s << std::endl;

                    return get_token(s, context, l);
                }
                context = special;
            } else if (
                    c == ',' || c == '=' || c == '>'
                    || c == ')' || c == '(' || c == ':'
                    ) {
                if (context != free) {
                    if (!fin.eof()) {
                        fin.unget();
                    }

                    std::cerr << s << std::endl;

                    return get_token(s, context, l);
                }

                context = special;
                s += c;
            } else if (c == '\'') {
                if (context != free) {
                    if (!fin.eof()) {
                        fin.unget();
                    }

                    std::cerr << s << std::endl;

                    return get_token(s, context, l);
                }

                context = string;
                s += c;
            } else if (
                    (c >= 'A' && c <= 'Z')
                    || (c >= 'a' && c <= 'z')
                    || (c >= '0' && c <= '9')
                    || c == '_'
                    ) {
                context = id;
                s += c;
            } else {
                return yy::parser::make_YYUNDEF(l);
            }
        }
    }


}