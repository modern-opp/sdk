#include<string>
#include<iostream>
#include "parser/parser.tab.hpp"
#include "util/token_utils.hpp"
#include "lexer/scanner.hpp"


namespace yy {

    parser::symbol_type yylex(Scanner &scanner) {
        parser::symbol_type &&token = scanner.get_token();
        std::cout << token_to_string(token.kind()) << std::endl;
        return std::forward<parser::symbol_type>(std::move(token));
    }


}