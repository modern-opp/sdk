//
// Created by Nikita Morozov on 27.03.2025.
//

#ifndef OPP_FRONTEND_SCANNER_HPP
#define OPP_FRONTEND_SCANNER_HPP

#include "parser/parser.tab.hpp"
#include "lexer/buffered_reader.hpp"

namespace yy {

    class Scanner {
    public:
        Scanner(BufferedReader reader, const std::string &filename);

        parser::symbol_type get_token();

    private:
        yy::parser::symbol_type get_literal_identifier_or_keyword();

        yy::parser::symbol_type get_string_literal();

        yy::parser::symbol_type get_num_literal();

        yy::parser::symbol_type get_special();

        yy::parser::symbol_type get_identifier_or_undef();

        char peek() const;

        char advance();

        void begin_token();

        yy::location end_token();

        yy::position begin_{};
        int lines_ = 0;
        int lineOffset_ = 0;
        std::string filename_;
        BufferedReader reader_;
    };
}

#endif //OPP_FRONTEND_SCANNER_HPP
