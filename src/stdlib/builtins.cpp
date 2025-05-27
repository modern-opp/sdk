//
// Created by Nikita Morozov on 27.05.2025.
//

#include "stdlib/builtins.hpp"

namespace oppstd {

    void register_builtins(SymbolTable *root_table) {
        root_table->add_symbol(string_class, std::make_unique<ClassSymbol>(string_class, nullptr));
        root_table->add_symbol(integer_class, std::make_unique<ClassSymbol>(integer_class, nullptr));
        root_table->add_symbol(real_class, std::make_unique<ClassSymbol>(real_class, nullptr));
        root_table->add_symbol(bool_class, std::make_unique<ClassSymbol>(bool_class, nullptr));
    }
}