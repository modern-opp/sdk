#include "ast.hpp"
#include "ast_utils.hpp"
#include <format>

//
// Created by Nikita Morozov on 31.03.2025.
//
void yy::PrettyPrintVisitor::operator()(const NodeBase &node_base) {

}

void yy::PrettyPrintVisitor::operator()(const Expr &expr) {

}

void yy::PrettyPrintVisitor::operator()(const PrimaryExpr &primary_expr) {

}

void yy::PrettyPrintVisitor::operator()(const BooleanLiteralExpr &boolean_literal_expr) {

}

void yy::PrettyPrintVisitor::operator()(const IntegerLiteralExpr &integer_literal_expr) {

}

void yy::PrettyPrintVisitor::operator()(const RealLiteralExpr &real_literal_expr) {

}

void yy::PrettyPrintVisitor::operator()(const StringLiteralExpr &string_literal_expr) {

}

void yy::PrettyPrintVisitor::operator()(const ThisExpr &this_expr) {

}

void yy::PrettyPrintVisitor::operator()(const MemberAccessExpr &member_access_expr) {

}

void yy::PrettyPrintVisitor::operator()(const FieldAccessExpr &field_access_expr) {

}

void yy::PrettyPrintVisitor::operator()(const MethodCallExpr &method_call_expr) {

}

void yy::PrettyPrintVisitor::operator()(const MemberAccess &member_access) {

}

void yy::PrettyPrintVisitor::operator()(const BodyExpr &body_expr) {

}

void yy::PrettyPrintVisitor::operator()(const Body &body) {

}

void yy::PrettyPrintVisitor::operator()(const Stmt &stmt) {

}

void yy::PrettyPrintVisitor::operator()(const ReturnStmt &return_stmt) {

}

void yy::PrettyPrintVisitor::operator()(const AssignmentStmt &assignment_stmt) {

}

void yy::PrettyPrintVisitor::operator()(const IfStmt &if_stmt) {

}

void yy::PrettyPrintVisitor::operator()(const WhileStmt &while_stmt) {

}

void yy::PrettyPrintVisitor::operator()(const MemberDeclarationExpr &member_declaration_expr) {

}

void yy::PrettyPrintVisitor::operator()(const MemberDeclaration &member_declaration) {

}

void yy::PrettyPrintVisitor::operator()(const ParameterDeclaration &parameter_declaration) {

}

void yy::PrettyPrintVisitor::operator()(const VariableDeclaration &variable_declaration) {

}

void yy::PrettyPrintVisitor::operator()(const ConstructorDeclaration &constructor_declaration) {

}

void yy::PrettyPrintVisitor::operator()(const ConstructorDefinition &constructor_definition) {

}

void yy::PrettyPrintVisitor::operator()(const MethodDeclaration &method_declaration) {

}

void yy::PrettyPrintVisitor::operator()(const MethodDefinition &method_definition) {

}

void yy::PrettyPrintVisitor::operator()(const ProgramDeclarationExpr &program_declaration_expr) {

}

void yy::PrettyPrintVisitor::operator()(const ProgramDeclaration &program_declaration) {
    std::for_each(
            program_declaration.class_declarations().begin(),
            program_declaration.class_declarations().end(),
            [this](auto &decl) {
                decl->accept(*this);
            }
    );
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
    class_definition.header()->accept(*this);
    depth_++;
    class_definition.body()->accept(*this);
    depth_--;
}

void yy::PrettyPrintVisitor::operator()(const Program &program) {
    program.class_declarations()->accept(*this);
    program.main_class()->accept(*this);
}

std::string yy::PrettyPrintVisitor::output() const {
    return output_;
}

void yy::PrettyPrintVisitor::print_ident() {
    output_ += std::string(depth_, '|');
    output_ += "*";
}

yy::PrettyPrintVisitor::~PrettyPrintVisitor()  = default;
