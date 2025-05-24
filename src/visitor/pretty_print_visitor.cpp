#include "ast/ast.hpp"
#include "visitor/pretty_print_visitor.hpp"
#include <algorithm>
#include <format>
#include <iostream>

//
// Created by Nikita Morozov on 31.03.2025.
//
void yy::PrettyPrintVisitor::operator()(const BooleanLiteralExpr &boolean_literal_expr) {
    print_ident();
    output_ += std::format(
            "BOOLEAN_LITERAL[value: {}]",
            boolean_literal_expr.value()
    );
}

void yy::PrettyPrintVisitor::operator()(const IntegerLiteralExpr &integer_literal_expr) {
    print_ident();
    output_ += std::format(
            "INTEGER_LITERAL[value: {}]",
            integer_literal_expr.value()
    );
}

void yy::PrettyPrintVisitor::operator()(const RealLiteralExpr &real_literal_expr) {
    print_ident();
    output_ += std::format(
            "REAL_LITERAL[value: {}]",
            real_literal_expr.value()
    );
}

void yy::PrettyPrintVisitor::operator()(const StringLiteralExpr &string_literal_expr) {
    print_ident();
    output_ += std::format(
            "STRING_LITERAL[value: {}]",
            string_literal_expr.value()
    );
}

void yy::PrettyPrintVisitor::operator()(const ThisExpr &this_expr) {
    print_ident();
    output_ += "THIS";
}

void yy::PrettyPrintVisitor::operator()(const FieldAccessExpr &field_access_expr) {
    print_ident();
    output_ += std::format(
            "FIELD_ACCESS_EXPR[name: {}]",
            field_access_expr.name()
    );
}

void yy::PrettyPrintVisitor::operator()(const MethodCallExpr &method_call_expr) {
    print_ident();
    output_ += std::format(
            "METHOD_CALL_EXPR[name: {}]",
            method_call_expr.name()
    );
    depth_++;
    std::for_each(
            method_call_expr.arguments().begin(),
            method_call_expr.arguments().end(),
            [this](auto &decl) {
                decl->accept(*this);
            }
    );
    depth_--;
    if (!method_call_expr.arguments().empty()) {
        print_close_ident();
    }
}

void yy::PrettyPrintVisitor::operator()(const MemberAccess &member_access) {
    print_ident();
    output_ += "MEMBER_ACCESS";
    depth_++;
    member_access.lhs()->accept(*this);
    member_access.rhs()->accept(*this);
    depth_--;
    print_close_ident();
}

void yy::PrettyPrintVisitor::operator()(const Body &body) {
    print_ident();
    output_ += "BODY";
    depth_++;
    std::for_each(
            body.expressions().begin(),
            body.expressions().end(),
            [this](auto &decl) {
                decl->accept(*this);
            }
    );
    depth_--;
    if (!body.expressions().empty()) {
        print_close_ident();
    }
}

void yy::PrettyPrintVisitor::operator()(const ReturnStmt &return_stmt) {
    print_ident();
    output_ += "RETURN";
    depth_++;
    return_stmt.expression()->accept(*this);
    depth_--;
    print_close_ident();
}

void yy::PrettyPrintVisitor::operator()(const AssignmentStmt &assignment_stmt) {
    print_ident();
    output_ += std::format(
            "ASSIGNMENT_STMT[name: {}]",
            assignment_stmt.name()
    );
    depth_++;
    assignment_stmt.expression()->accept(*this);
    depth_--;
    print_close_ident();
}

void yy::PrettyPrintVisitor::operator()(const IfStmt &if_stmt) {
    print_ident();
    output_ += "IF";
    depth_++;
    if_stmt.condition()->accept(*this);
    if_stmt.then_body()->accept(*this);
    if (if_stmt.else_body()) {
        if_stmt.else_body()->accept(*this);
    }
    depth_--;
    print_close_ident();
}

void yy::PrettyPrintVisitor::operator()(const WhileStmt &while_stmt) {
    print_ident();
    output_ += "WHILE";
    depth_++;
    while_stmt.condition()->accept(*this);
    while_stmt.loop_body()->accept(*this);
    depth_--;
    print_close_ident();
}

void yy::PrettyPrintVisitor::operator()(const MemberDeclaration &member_declaration) {
    print_ident();
    output_ += "MEMBER_DECLARATION";
    depth_++;
    std::for_each(
            member_declaration.member_declarations().begin(),
            member_declaration.member_declarations().end(),
            [this](auto &decl) {
                decl->accept(*this);
            }
    );
    depth_--;
    if (!member_declaration.member_declarations().empty()) {
        print_close_ident();
    }
}

void yy::PrettyPrintVisitor::operator()(const ParameterDeclaration &parameter_declaration) {
    print_ident();
    output_ += std::format(
            "PARAMETER_DECLARATION[name: {}, type: {}]",
            parameter_declaration.name(),
            parameter_declaration.type()
    );
}

void yy::PrettyPrintVisitor::operator()(const VariableDeclaration &variable_declaration) {
    print_ident();
    output_ += std::format(
            "VARIABLE_DECLARATION[name: {}]",
            variable_declaration.name()
    );
    depth_++;
    variable_declaration.initializer()->accept(*this);
    depth_--;
    print_close_ident();
}

void yy::PrettyPrintVisitor::operator()(const ConstructorDeclaration &constructor_declaration) {
    print_ident();
    output_ += "CONSTRUCTOR_DECLARATION";
    depth_++;
    std::for_each(
            constructor_declaration.parameters().begin(),
            constructor_declaration.parameters().end(),
            [this](auto &decl) {
                decl->accept(*this);
            }
    );
    depth_--;
    if (!constructor_declaration.parameters().empty()) {
        print_close_ident();
    }
}

void yy::PrettyPrintVisitor::operator()(const ConstructorDefinition &constructor_definition) {
    print_ident();
    output_ += "CONSTRUCTOR_DEFINITION";
    depth_++;
    constructor_definition.header()->accept(*this);
    constructor_definition.body()->accept(*this);
    depth_--;
    print_close_ident();
}

void yy::PrettyPrintVisitor::operator()(const MethodDeclaration &method_declaration) {
    print_ident();
    output_ += std::format(
            "METHOD_DECLARATION[name: {}, returnType: {}]",
            method_declaration.name(),
            method_declaration.return_type()
    );
    depth_++;
    std::for_each(
            method_declaration.parameters().begin(),
            method_declaration.parameters().end(),
            [this](auto &decl) {
                decl->accept(*this);
            }
    );
    depth_--;
    print_close_ident();
}

void yy::PrettyPrintVisitor::operator()(const MethodDefinition &method_definition) {
    print_ident();
    output_ += "METHOD_DEFINITION";
    depth_++;
    method_definition.header()->accept(*this);
    method_definition.body()->accept(*this);
    depth_--;
    print_close_ident();
}

void yy::PrettyPrintVisitor::operator()(const ProgramDeclaration &program_declaration) {
    print_ident();
    output_ += "PROGRAM_DECLARATION";
    depth_++;
    std::for_each(
            program_declaration.class_declarations().begin(),
            program_declaration.class_declarations().end(),
            [this](auto &decl) {
                decl->accept(*this);
            }
    );
    depth_--;
//    print_close_ident();
}

void yy::PrettyPrintVisitor::operator()(const ClassDeclaration &class_declaration) {
    print_ident();
    output_ += std::format(
            "CLASS_DECLARATION[name: {} parent: {}]",
            class_declaration.name(),
            class_declaration.parent()
    );
}

void yy::PrettyPrintVisitor::operator()(const ClassDefinition &class_definition) {
    print_ident();
    output_ += "CLASS_DEFINITION";
    depth_++;
    class_definition.header()->accept(*this);
    class_definition.body()->accept(*this);
    depth_--;
    print_close_ident();
}

void yy::PrettyPrintVisitor::operator()(const Program &program) {
    output_ += "*PROGRAM";
    depth_++;
    program.class_declarations()->accept(*this);
    program.main_class()->accept(*this);
    output_ += "\n*";
}

std::string yy::PrettyPrintVisitor::output() const {
    return output_;
}

void yy::PrettyPrintVisitor::print_ident() {
    output_ += "\n";
    output_ += "|";
    for (size_t i = 0; i < depth_ - 1; i++) {
        output_ += " |";
    }
    output_ += "-*";
}

void yy::PrettyPrintVisitor::print_close_ident() {
    output_ += "\n";
    output_ += "|";
    for (size_t i = 0; depth_ >= 2 && i < depth_ - 2; i++) {

        output_ += " |";
    }
    output_ += " *";
}

