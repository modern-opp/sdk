//
// Created by Nikita Morozov on 03.04.2025.
//

#include "visitors/recursive_visitor.hpp"

void yy::RecursiveVisitor::operator()(const BooleanLiteralExpr &boolean_literal_expr) {

}

void yy::RecursiveVisitor::operator()(const IntegerLiteralExpr &integer_literal_expr) {

}

void yy::RecursiveVisitor::operator()(const RealLiteralExpr &real_literal_expr) {

}

void yy::RecursiveVisitor::operator()(const StringLiteralExpr &string_literal_expr) {

}

void yy::RecursiveVisitor::operator()(const ThisExpr &this_expr) {

}

void yy::RecursiveVisitor::operator()(const FieldAccessExpr &field_access_expr) {

}

void yy::RecursiveVisitor::operator()(const MethodCallExpr &method_call_expr) {
    std::for_each(
            method_call_expr.arguments().begin(),
            method_call_expr.arguments().end(),
            [this](auto &ptr) {
                ptr->accept(*this);
            }
    );
}

void yy::RecursiveVisitor::operator()(const MemberAccess &member_access) {
    member_access.lhs()->accept(*this);
    member_access.rhs()->accept(*this);
}

void yy::RecursiveVisitor::operator()(const Body &body) {
    std::for_each(
            body.expressions().begin(),
            body.expressions().end(),
            [this](auto &ptr) {
                ptr->accept(*this);
            }
    );
}

void yy::RecursiveVisitor::operator()(const ReturnStmt &return_stmt) {
    return_stmt.expression()->accept(*this);
}

void yy::RecursiveVisitor::operator()(const AssignmentStmt &assignment_stmt) {
    assignment_stmt.expression()->accept(*this);
}

void yy::RecursiveVisitor::operator()(const IfStmt &if_stmt) {
    if_stmt.condition()->accept(*this);
    if_stmt.then_body()->accept(*this);
    if_stmt.else_body()->accept(*this);
}

void yy::RecursiveVisitor::operator()(const WhileStmt &while_stmt) {
    while_stmt.condition()->accept(*this);
    while_stmt.loop_body()->accept(*this);
}

void yy::RecursiveVisitor::operator()(const MemberDeclaration &member_declaration) {
    std::for_each(
            member_declaration.member_declarations().begin(),
            member_declaration.member_declarations().end(),
            [this](auto &ptr) {
                ptr->accept(*this);
            }
    );
}

void yy::RecursiveVisitor::operator()(const ParameterDeclaration &parameter_declaration) {

}

void yy::RecursiveVisitor::operator()(const VariableDeclaration &variable_declaration) {
    variable_declaration.initializer()->accept(*this);
}

void yy::RecursiveVisitor::operator()(const ConstructorDeclaration &constructor_declaration) {
    std::for_each(
            constructor_declaration.parameters().begin(),
            constructor_declaration.parameters().end(),
            [this](auto &ptr) {
                ptr->accept(*this);
            }
    );
}

void yy::RecursiveVisitor::operator()(const ConstructorDefinition &constructor_definition) {
    constructor_definition.header()->accept(*this);
    constructor_definition.body()->accept(*this);
}

void yy::RecursiveVisitor::operator()(const MethodDeclaration &method_declaration) {
    std::for_each(
            method_declaration.parameters().begin(),
            method_declaration.parameters().end(),
            [this](auto &ptr) {
                ptr->accept(*this);
            }
    );
}

void yy::RecursiveVisitor::operator()(const MethodDefinition &method_definition) {
    method_definition.header()->accept(*this);
    method_definition.body()->accept(*this);
}

void yy::RecursiveVisitor::operator()(const ProgramDeclaration &program_declaration) {
    std::for_each(
            program_declaration.class_declarations().begin(),
            program_declaration.class_declarations().end(),
            [this](auto &ptr) {
                ptr->accept(*this);
            }
    );
}

void yy::RecursiveVisitor::operator()(const ClassDeclaration &class_declaration) {

}

void yy::RecursiveVisitor::operator()(const ClassDefinition &class_definition) {
    class_definition.header()->accept(*this);
    class_definition.body()->accept(*this);
}

void yy::RecursiveVisitor::operator()(const Program &program) {
    program.class_declarations()->accept(*this);
    program.main_class()->accept(*this);
}

