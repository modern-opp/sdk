//
// Created by Nikita Morozov on 27.05.2025.
//

#ifndef OPP_FRONTEND_BUILTINS_HPP
#define OPP_FRONTEND_BUILTINS_HPP

#include "semantic/symbol_table.hpp"
#include <string>

namespace oppstd {
    inline constexpr std::string string_class = "String";
    inline constexpr std::string integer_class = "Integer";
    inline constexpr std::string real_class = "Real";
    inline constexpr std::string bool_class = "Boolean";

    void register_builtins(SymbolTable* root_table);
}



#endif //OPP_FRONTEND_BUILTINS_HPP
