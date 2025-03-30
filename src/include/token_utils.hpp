//
// Created by Nikita Morozov on 30.03.2025.
//

#ifndef OPP_FRONTEND_TOKEN_UTILS_HPP
#define OPP_FRONTEND_TOKEN_UTILS_HPP

#include <string>
#include "parser.tab.hpp"

namespace yy {
    std::string token_to_string(yy::parser::symbol_kind::symbol_kind_type token);
}
#endif //OPP_FRONTEND_TOKEN_UTILS_HPP
