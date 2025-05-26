//
// Created by Nikita Morozov on 26.05.2025.
//

#include "semantic/symbol_table_class_collector_visitor.hpp"

#include "ast/ast.hpp"
#include "visitor/pretty_print_visitor.hpp"
#include <algorithm>
#include <format>
#include <iostream>

//
// Created by Nikita Morozov on 31.03.2025.
//


void yy::SymbolTableClassCollectorVisitor::operator()(const ClassDefinition &class_definition) {
    auto symbol = std::make_unique<ClassSymbol>(
            class_definition.header()->name(),
            &class_definition
    );
    scope_symbol_table_->add_symbol(class_definition.header()->name(), std::move(symbol));
    result_ = scope_symbol_table_;
}



