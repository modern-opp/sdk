//
// Created by Nikita Morozov on 26.05.2025.
//

#include "semantic/symbol_table_method_collector_visitor.hpp"
#include "semantic/mangling_transformer.hpp"

void yy::SymbolTableMethodCollectorVisitor::operator()(const VariableDeclaration &variable_declaration) {
    // todo: inheritanse issue
    auto symbol = std::make_unique<InstanceSymbol>(
            field,
            nullptr,
            variable_declaration.name(),
            dynamic_cast<const MemberDeclarationExpr*>(&variable_declaration)
    );

    scope_symbol_table_->add_symbol(variable_declaration.name(), std::move(symbol));
}

void yy::SymbolTableMethodCollectorVisitor::operator()(const ConstructorDefinition &definition) {
    add_method(
            constructor_definition,
            scope_symbol_table_->resolve_this()->name(),
            definition.header()->parameters(),
            scope_symbol_table_->resolve_this(),
            &definition
    );
}

void yy::SymbolTableMethodCollectorVisitor::operator()(const ConstructorDeclaration &declaration) {
    add_method(
            constructor_declaration,
            scope_symbol_table_->resolve_this()->name(),
            declaration.parameters(),
            scope_symbol_table_->resolve_this(),
            &declaration
    );
}


void yy::SymbolTableMethodCollectorVisitor::operator()(const MethodDeclaration &declaration) {
    add_method(
            method_declaration,
            declaration.name(),
            declaration.parameters(),
            scope_symbol_table_->resolve_class(declaration.return_type()),
            &declaration
    );
}


void yy::SymbolTableMethodCollectorVisitor::operator()(const MethodDefinition &definition) {
    add_method(
            method_definition,
            definition.header()->name(),
            definition.header()->parameters(),
            scope_symbol_table_->resolve_class(definition.header()->return_type()),
            &definition
    );
}

void yy::SymbolTableMethodCollectorVisitor::operator()(const ClassDeclaration &class_declaration) {
    result_ = scope_symbol_table_->resolve_symbol(class_declaration.name());
}

void yy::SymbolTableMethodCollectorVisitor::operator()(const ClassDefinition &class_definition) {
    auto child = class_definition.header()->accept(*this);
    auto backup = scope_symbol_table_;
    scope_symbol_table_ = child;
    class_definition.body()->accept(*this);
    scope_symbol_table_ = backup;
    result_ = scope_symbol_table_;
}

void yy::SymbolTableMethodCollectorVisitor::add_method(
        MethodSymbolKind kind,
        const std::string &method_name,
        const std::vector<std::unique_ptr<ParameterDeclaration>> &parameters,
        const ClassSymbol *return_type,
        const NodeBase *node
) {
    std::vector<ClassSymbol *> params;
    std::for_each(
            parameters.begin(),
            parameters.end(),
            [&params, this](auto &param) {
                auto clazz = this->scope_symbol_table_->resolve_class(param->type());
                params.push_back(clazz);
            }
    );

    auto name = transform_to_mangling_name(method_name, params);
    auto symbol = std::make_unique<MethodSymbol>(
            kind,
            scope_symbol_table_->resolve_this(),
            std::move(params),
            return_type,
            method_name,
            node
    );

    scope_symbol_table_->add_symbol(name, std::move(symbol));
}

