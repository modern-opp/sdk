//
// Created by Nikita Morozov on 27.05.2025.
//

#include "stdlib/builtins.hpp"

namespace oppstd {

    void register_builtins(SymbolTable *root_table) {
        auto int_cls = std::make_unique<ClassSymbol>(IntegerNames::kClass, nullptr);


        auto int_tbl = root_table->add_symbol(IntegerNames::kClass, std::move(int_cls));

        int_tbl->add_symbol(
                IntegerNames::kPlus,
                std::make_unique<MethodSymbol>(
                        method_declaration,
                        IntegerNames::kPlus,
                        {},
                        int_cls.get(),
                        IntegerNames::kPlus,
                        nullptr,
                )
        );

        int_tbl->add_symbol(
                IntegerNames::kPlus,
                std::make_unique<MethodSymbol>(
                        method_declaration,
                        IntegerNames::kPlus,
                        {},
                        int_cls.get(),
                        IntegerNames::kPlus,
                        nullptr,
                )
        );


        auto real_tbl = root_table->add_symbol(
                RealNames::kClass,
                std::make_unique<ClassSymbol>(RealNames::kClass, nullptr)
        );
        auto bool_tbl = root_table->add_symbol(
                BooleanNames::kClass,
                std::make_unique<ClassSymbol>(BooleanNames::kClass, nullptr)
        );
        auto string_tbl = root_table->add_symbol(
                StringNames::kClass,
                std::make_unique<ClassSymbol>(StringNames::kClass, nullptr)
        );
    }
}