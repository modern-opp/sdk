//
// Created by Nikita Morozov on 27.05.2025.
//

#include "semantic/inheritance_visitor.hpp"
#include <unordered_set>


void yy::InheritanceVisitor::operator()(const VariableDeclaration &variable_declaration) {
    auto *location = dynamic_cast<const MemberDeclarationExpr *>(&variable_declaration);
    auto &symbol_table = symbol_table_index_->restore(location);
    auto variable = symbol_table.resolve_local(variable_declaration.name());

    if (!variable) {
        semantic_errors_.emplace_back(
                "Unexpected identifier: " + variable_declaration.name(),
                location->location()
        );
        result_ = nullptr;
        return;
    }

    auto class_scope = symbol_table.resolve_this();
    const ClassDefinition *parent;
    for (auto clazz = symbol_table.resolve_symbol(class_scope->declaredNode()->header()->parent());
         clazz != nullptr;
         (parent = dynamic_cast<const ClassDefinition *>(clazz->get_symbol()->declaredNode()),
                 clazz = symbol_table.resolve_symbol(parent->header()->parent()))
            ) {
        auto super_local = clazz->resolve_local(variable_declaration.name());
        if (super_local && variable->clazz() != super_local->clazz()) {
            semantic_errors_.emplace_back(
                    "Redefinition of field: " + variable_declaration.name(),
                    location->location()
            );
        }

    }
}


void yy::InheritanceVisitor::operator()(const ClassDeclaration &class_declaration) {
    auto &symbol_table = symbol_table_index_->restore(&class_declaration);

    ClassSymbol *clazz = symbol_table.resolve_class(class_declaration.name());
    if (clazz ==
        symbol_table.resolve_class(class_declaration.parent())) {
        semantic_errors_.emplace_back(
                "Self inheritance forbidden",
                class_declaration.location()
        );
        return;
    }

    if (!clazz) {
        semantic_errors_.emplace_back(
                "Missing class: " + class_declaration.name(),
                class_declaration.location()
        );
    }

    std::unordered_set<std::string> super_graph;

    for (; clazz != nullptr; (clazz = symbol_table.resolve_class(clazz->declaredNode()->header()->parent()))) {
        if (auto twiceParent = super_graph.find(clazz->name()); twiceParent != super_graph.end()) {
            semantic_errors_.emplace_back(
                    "Class loop on : " + (*twiceParent),
                    class_declaration.location()
            );
            break;
        }
        super_graph.emplace(clazz->name());
    }
}
