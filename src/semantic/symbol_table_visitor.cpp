//
// Created by Nikita Morozov on 26.05.2025.
//

#include <algorithm>
#include <format>

#include "semantic/symbol_table_visitor.hpp"
#include "ast/ast.hpp"
#include "semantic/mangling_transformer.hpp"


//
// Created by Nikita Morozov on 31.03.2025.
//
void yy::SymbolTableVisitor::operator()(const BooleanLiteralExpr &boolean_literal_expr) {
    symbol_table_index_->commit(&boolean_literal_expr, scope_symbol_table_);
}

void yy::SymbolTableVisitor::operator()(const IntegerLiteralExpr &integer_literal_expr) {
    symbol_table_index_->commit(&integer_literal_expr, scope_symbol_table_);
}

void yy::SymbolTableVisitor::operator()(const RealLiteralExpr &real_literal_expr) {
    symbol_table_index_->commit(&real_literal_expr, scope_symbol_table_);
}

void yy::SymbolTableVisitor::operator()(const StringLiteralExpr &string_literal_expr) {
    symbol_table_index_->commit(&string_literal_expr, scope_symbol_table_);
}

void yy::SymbolTableVisitor::operator()(const ThisExpr &this_expr) {
    symbol_table_index_->commit(&this_expr, scope_symbol_table_);
}

void yy::SymbolTableVisitor::operator()(const FieldAccessExpr &field_access_expr) {
    symbol_table_index_->commit(&field_access_expr, scope_symbol_table_);
}

void yy::SymbolTableVisitor::operator()(const MethodCallExpr &method_call_expr) {
    symbol_table_index_->commit(&method_call_expr, scope_symbol_table_);
    std::for_each(
            method_call_expr.arguments().begin(),
            method_call_expr.arguments().end(),
            [this](auto &decl) {
                decl->accept(*this);
            }
    );

}

void yy::SymbolTableVisitor::operator()(const MemberAccess &member_access) {
    symbol_table_index_->commit(&member_access, scope_symbol_table_);
    member_access.lhs()->accept(*this);
    member_access.rhs()->accept(*this);
}

void yy::SymbolTableVisitor::operator()(const Body &body) {
    auto parent_scope = scope_symbol_table_;
    symbol_table_index_->commit(&body, scope_symbol_table_);
    scope_symbol_table_ = parent_scope->add_child();
    std::for_each(
            body.expressions().begin(),
            body.expressions().end(),
            [this](auto &decl) {
                scope_symbol_table_ = decl->accept(*this);
            }
    );
    scope_symbol_table_ = parent_scope;
}

void yy::SymbolTableVisitor::operator()(const ReturnStmt &return_stmt) {
    symbol_table_index_->commit(&return_stmt, scope_symbol_table_);
    return_stmt.expression()->accept(*this);
}

void yy::SymbolTableVisitor::operator()(const AssignmentStmt &assignment_stmt) {
    symbol_table_index_->commit(&assignment_stmt, scope_symbol_table_);
    assignment_stmt.expression()->accept(*this);
}

void yy::SymbolTableVisitor::operator()(const IfStmt &if_stmt) {
    symbol_table_index_->commit(&if_stmt, scope_symbol_table_);
    if_stmt.condition()->accept(*this);

    if_stmt.then_body()->accept(*this);

    if (if_stmt.else_body()) {
        if_stmt.else_body()->accept(*this);
    }
}

void yy::SymbolTableVisitor::operator()(const WhileStmt &while_stmt) {
    symbol_table_index_->commit(&while_stmt, scope_symbol_table_);
    while_stmt.condition()->accept(*this);
    while_stmt.loop_body()->accept(*this);
}

void yy::SymbolTableVisitor::operator()(const MemberDeclaration &member_declaration) {
    symbol_table_index_->commit(&member_declaration, scope_symbol_table_);
    std::for_each(
            member_declaration.member_declarations().begin(),
            member_declaration.member_declarations().end(),
            [this](auto &decl) {
                decl->accept(*this);
            }
    );
}

void yy::SymbolTableVisitor::operator()(const ParameterDeclaration &parameter_declaration) {
    auto previous = scope_symbol_table_->resolve_local(parameter_declaration.name());
    if (previous) {
        semantic_errors_.emplace_back(
                "Symbol already defined: " + parameter_declaration.name(),
                parameter_declaration.location()
        );
        result_ = scope_symbol_table_;
        return;
    }

    auto symbol = std::make_unique<InstanceSymbol>(
            parm,
            scope_symbol_table_->resolve_class(parameter_declaration.type()),
            parameter_declaration.name(),
            &parameter_declaration
    );
    auto child_scope = scope_symbol_table_->add_symbol(parameter_declaration.name(), std::move(symbol));
    symbol_table_index_->commit(&parameter_declaration, child_scope);
    result_ = child_scope;
}

void yy::SymbolTableVisitor::operator()(const VariableDeclaration &variable_declaration) {
    auto field_scope = scope_symbol_table_->resolve_symbol(variable_declaration.name());
    SymbolTable* child_scope = nullptr;
    auto *location = dynamic_cast<const MemberDeclarationExpr *>(&variable_declaration);
    if (field_scope) {
        auto field_symbol = dynamic_cast<InstanceSymbol*>(field_scope->get_symbol());
        if (field_symbol->kind() != field) {
            semantic_errors_.emplace_back(
                    "Symbol already defined: " + variable_declaration.name(),
                    location->location()
            );
            result_ = scope_symbol_table_;
            return;
        }

        child_scope = field_scope;
        symbol_table_index_->commit(location, child_scope);
    } else {
        auto symbol = std::make_unique<InstanceSymbol>(
                local,
                nullptr,
                variable_declaration.name(),
                location
        );
        child_scope = scope_symbol_table_->add_symbol(variable_declaration.name(), std::move(symbol));
        if (!child_scope) {
            semantic_errors_.emplace_back(
                    "Symbol already defined: " + variable_declaration.name(),
                    location->location()
            );
            result_ = scope_symbol_table_;
            return;
        }
        symbol_table_index_->commit(location, child_scope);

    }
    variable_declaration.initializer()->accept(*this);
    result_ = child_scope;
}

void yy::SymbolTableVisitor::operator()(const ConstructorDeclaration &declaration) {
    auto parent_scope = scope_symbol_table_;
    auto method_scope = resolve_method(
            parent_scope->resolve_this()->name(),
            declaration.parameters()
    );
    scope_symbol_table_ = method_scope;
    symbol_table_index_->commit(&declaration, scope_symbol_table_);
    std::for_each(
            declaration.parameters().begin(),
            declaration.parameters().end(),
            [this](auto &ptr) {
                scope_symbol_table_ = ptr->accept(*this);
            }
    );
    result_ = scope_symbol_table_;

    scope_symbol_table_ = parent_scope;
}

void yy::SymbolTableVisitor::operator()(const ConstructorDefinition &definition) {
    auto parent_scope = scope_symbol_table_;

    auto method_scope = resolve_method(
            parent_scope->resolve_this()->name(),
            definition.header()->parameters()
    );
    scope_symbol_table_ = method_scope;
    symbol_table_index_->commit(&definition, scope_symbol_table_);
    auto child_scope = definition.header()->accept(*this);
    scope_symbol_table_ = child_scope;
    definition.body()->accept(*this);

    scope_symbol_table_ = parent_scope;
}

void yy::SymbolTableVisitor::operator()(const MethodDeclaration &declaration) {
    auto parent_scope = scope_symbol_table_;

    auto method_scope = resolve_method(
            declaration.name(),
            declaration.parameters()
    );
    scope_symbol_table_ = method_scope;
    symbol_table_index_->commit(&declaration, scope_symbol_table_);
    std::for_each(
            declaration.parameters().begin(),
            declaration.parameters().end(),
            [this](auto &ptr) {
                scope_symbol_table_ = ptr->accept(*this);
            }
    );
    result_ = scope_symbol_table_;

    scope_symbol_table_ = parent_scope;
}

void yy::SymbolTableVisitor::operator()(const MethodDefinition &definition) {
    auto parent_scope = scope_symbol_table_;

    auto method_scope = resolve_method(
            definition.header()->name(),
            definition.header()->parameters()
    );
    scope_symbol_table_ = method_scope;
    symbol_table_index_->commit(&definition, scope_symbol_table_);
    auto child_scope = definition.header()->accept(*this);
    scope_symbol_table_ = child_scope;
    definition.body()->accept(*this);

    scope_symbol_table_ = parent_scope;
}

void yy::SymbolTableVisitor::operator()(const ProgramDeclaration &program_declaration) {
    symbol_table_index_->commit(&program_declaration, scope_symbol_table_);
    std::for_each(
            program_declaration.class_declarations().begin(),
            program_declaration.class_declarations().end(),
            [this](auto &decl) {
                decl->accept(*this);
            }
    );
}

void yy::SymbolTableVisitor::operator()(const ClassDeclaration &class_declaration) {
    symbol_table_index_->commit(&class_declaration, scope_symbol_table_);
}

void yy::SymbolTableVisitor::operator()(const ClassDefinition &class_definition) {
    auto parent_scope = scope_symbol_table_;
    auto child_scope = parent_scope->resolve_symbol(class_definition.header()->name());
    scope_symbol_table_ = child_scope;
    symbol_table_index_->commit(&class_definition, scope_symbol_table_);
    class_definition.header()->accept(*this);
    class_definition.body()->accept(*this);
    scope_symbol_table_ = parent_scope;
}

void yy::SymbolTableVisitor::operator()(const Program &program) {
    symbol_table_index_->commit(&program, scope_symbol_table_);
    program.class_declarations()->accept(*this);
    program.main_class()->accept(*this);
}

SymbolTable *yy::SymbolTableVisitor::resolve_method(
        const std::string &method_name,
        const std::vector<std::unique_ptr<ParameterDeclaration>> &parameters
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
    auto class_name = scope_symbol_table_->resolve_this()->name();
    auto class_scope = scope_symbol_table_->resolve_symbol(class_name);
    auto name = transform_to_mangling_name(method_name, params);
    return class_scope->resolve_symbol(name);
}

