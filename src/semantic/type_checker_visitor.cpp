//
// Created by Nikita Morozov on 27.05.2025.
//

#include "semantic/type_checker_visitor.hpp"

#include <algorithm>
#include <format>

#include "ast/ast.hpp"
#include "semantic/mangling_transformer.hpp"
#include "stdlib/builtins.hpp"


void yy::TypeCheckerVisitor::operator()(const BooleanLiteralExpr &boolean_literal_expr) {
    auto &symbol_table = symbol_table_index_->restore(&boolean_literal_expr);
    result_ = symbol_table.resolve_class(oppstd::bool_class);
}

void yy::TypeCheckerVisitor::operator()(const IntegerLiteralExpr &integer_literal_expr) {
    auto &symbol_table = symbol_table_index_->restore(&integer_literal_expr);
    result_ = symbol_table.resolve_class(oppstd::integer_class);
}

void yy::TypeCheckerVisitor::operator()(const RealLiteralExpr &real_literal_expr) {
    auto &symbol_table = symbol_table_index_->restore(&real_literal_expr);
    result_ = symbol_table.resolve_class(oppstd::real_class);
}

void yy::TypeCheckerVisitor::operator()(const StringLiteralExpr &string_literal_expr) {
    auto &symbol_table = symbol_table_index_->restore(&string_literal_expr);
    result_ = symbol_table.resolve_class(oppstd::string_class);
}

void yy::TypeCheckerVisitor::operator()(const ThisExpr &this_expr) {
    auto &symbol_table = symbol_table_index_->restore(&this_expr);
    result_ = symbol_table.resolve_this();
}

void yy::TypeCheckerVisitor::operator()(const FieldAccessExpr &field_access_expr) {
    auto &symbol_table = symbol_table_index_->restore(&field_access_expr);
    if (callee_) {
        auto field = symbol_table.resolve_field(callee_->name(), field_access_expr.name());
        if (!field) {
            semantic_errors_.emplace_back("Missing property", field_access_expr.location());
            result_ = nullptr;
            return;
        }

        if (!field->clazz()) {
            semantic_errors_.emplace_back(
                    "Ambiguous variable type: " + field_access_expr.name(),
                    field_access_expr.location()
            );
            result_ = nullptr;
            return;
        }
        result_ = const_cast<ClassSymbol *>(field->clazz());

        return;
    }

    result_ = symbol_table.resolve_local(field_access_expr.name());
}

void yy::TypeCheckerVisitor::operator()(const MethodCallExpr &method_call_expr) {
    auto &symbol_table = symbol_table_index_->restore(&method_call_expr);
    std::vector<ClassSymbol *> args;
    bool has_error = false;
    std::for_each(
            method_call_expr.arguments().begin(),
            method_call_expr.arguments().end(),
            [&args, this, &has_error](auto &arg) {
                auto arg_type = dynamic_cast<ClassSymbol *>(arg->accept(*this));

                if (!arg_type) {
                    semantic_errors_.emplace_back("Cant get arg type", arg->location());
                    has_error = true;
                    return;
                }

                args.push_back(arg_type);
            });

    if (has_error) {
        result_ = nullptr;
        return;
    }


    if (callee_) {
        auto method = symbol_table.resolve_method(callee_->name(), method_call_expr.name(), args);
        if (!method) {
            semantic_errors_.emplace_back("Cant get overload for method", method_call_expr.location());
            result_ = nullptr;
            return;
        }
        result_ = const_cast<ClassSymbol *>(method->returnType());
        return;
    }

    callee_ = symbol_table.resolve_this();
    if (callee_) {
        auto method = symbol_table.resolve_method(callee_->name(), method_call_expr.name(), args);
        if (method) {
            result_ = const_cast<ClassSymbol *>(method->returnType());
            return;
        }
    }


    auto constructor = symbol_table.resolve_method(
            method_call_expr.name(),
            method_call_expr.name(),
            args
    );

    if (!constructor) {
        semantic_errors_.emplace_back("Cant get overload for constructor", method_call_expr.location());
        result_ = nullptr;
        return;
    }

    result_ = const_cast<ClassSymbol *>(constructor->returnType());
}

void yy::TypeCheckerVisitor::operator()(const MemberAccess &member_access) {
    auto rhs = member_access.lhs()->accept(*this);
    auto callee = dynamic_cast<ClassSymbol *>(rhs);
    if (callee) {
        callee_ = callee;
    }
    member_access.rhs()->accept(*this);
    callee_ = nullptr;
}


void yy::TypeCheckerVisitor::operator()(const ReturnStmt &return_stmt) {
    auto &symbol_table = symbol_table_index_->restore(&return_stmt);
    auto method_scope = symbol_table.method_scope();
    if (!method_scope) {
        semantic_errors_.emplace_back(
                "Unexpected return statement",
                return_stmt.location()
        );
        return;
    }

    auto type = return_stmt.expression()->accept(*this);
    if (!type && method_scope->returnType()) {
        semantic_errors_.emplace_back(
                "Expected type: " + method_scope->returnType()->name() + ", but found: nullptr",
                return_stmt.location()
        );
        return;
    }


    auto return_type = dynamic_cast<ClassSymbol *>(type);

    if (return_type != method_scope->returnType()) {
        auto expected = method_scope->returnType() ? method_scope->returnType()->name() : "void";
        auto actual = return_type ? return_type->name() : "nullptr(void)";
        semantic_errors_.emplace_back(
                "Expected type: " + expected + ", but found: " + actual,
                return_stmt.location()
        );
    }

}

void yy::TypeCheckerVisitor::operator()(const AssignmentStmt &assignment_stmt) {
    auto &symbol_table = symbol_table_index_->restore(&assignment_stmt);
    auto symbol = symbol_table.resolve_local(assignment_stmt.name());
    if (!symbol->clazz()) {
        semantic_errors_.emplace_back(
                "Ambiguous variable type: " + assignment_stmt.name(),
                assignment_stmt.location()
        );
    }

    auto expr_type = dynamic_cast<ClassSymbol *>(assignment_stmt.expression()->accept(*this));
    if (!expr_type) {
        semantic_errors_.emplace_back(
                "Ambiguous expression type: " + assignment_stmt.name(),
                assignment_stmt.location()
        );
        return;
    }

    if (symbol->clazz() && symbol->clazz() != expr_type) {
        semantic_errors_.emplace_back(
                "Expected type: " + symbol->clazz()->name() + ", but found: " + expr_type->name(),
                assignment_stmt.location()
        );
    }
}

void yy::TypeCheckerVisitor::operator()(const IfStmt &if_stmt) {
    auto &symbol_table = symbol_table_index_->restore(&if_stmt);
    auto condition = dynamic_cast<ClassSymbol *>(if_stmt.condition()->accept(*this));
    auto boolean = symbol_table.resolve_class(oppstd::bool_class);
    if (condition != boolean) {
        semantic_errors_.emplace_back(
                "Expected type: " + boolean->name() + ", but found: " + condition->name(),
                if_stmt.condition()->location()
        );
    }

    if_stmt.then_body()->accept(*this);

    if (if_stmt.else_body()) {
        if_stmt.else_body()->accept(*this);
    }
}

void yy::TypeCheckerVisitor::operator()(const WhileStmt &while_stmt) {
    auto &symbol_table = symbol_table_index_->restore(&while_stmt);
    auto condition = dynamic_cast<ClassSymbol *>(while_stmt.condition()->accept(*this));
    auto boolean = symbol_table.resolve_class(oppstd::bool_class);
    if (condition != boolean) {
        semantic_errors_.emplace_back(
                "Expected type: " + boolean->name() + ", but found: " + condition->name(),
                while_stmt.condition()->location()
        );
    }
    while_stmt.loop_body()->accept(*this);
}


void yy::TypeCheckerVisitor::operator()(const ParameterDeclaration &parameter_declaration) {
    auto &symbol_table = symbol_table_index_->restore(&parameter_declaration);
    auto scope = symbol_table.resolve_this();
    if (symbol_table.resolve_local(parameter_declaration.name())->clazz() == scope) {
        semantic_errors_.emplace_back(
                "Recursive type in constructor: " + scope->name(),
                parameter_declaration.location()
        );
    }
}

void yy::TypeCheckerVisitor::operator()(const VariableDeclaration &variable_declaration) {
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
    auto type = dynamic_cast<ClassSymbol *>(variable_declaration.initializer()->accept(*this));
    if (!type) {
        semantic_errors_.emplace_back(
                "Can`t infer type: " + variable_declaration.name(),
                location->location()
        );
        result_ = nullptr;
        return;
    }

    ClassSymbol *class_scope = symbol_table.resolve_this();
    if (type == class_scope) {
        semantic_errors_.emplace_back(
                "Recursive type forbidden: " + variable_declaration.name(),
                location->location()
        );
        result_ = nullptr;
        return;
    }


    variable->setClazz(type);
    result_ = type;
}


