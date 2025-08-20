//
// Created by Nikita Morozov on 14.06.2025.
//

#include "codegen/codegen_visitor.hpp"


#include "ast/ast.hpp"
#include <algorithm>
#include <format>
#include <iostream>

void yy::CodegenVisitor::operator()(const BooleanLiteralExpr &boolean_literal_expr) {

}

void yy::CodegenVisitor::operator()(const IntegerLiteralExpr &integer_literal_expr) {
    print_ident();
    output_ += std::format(
            "INTEGER_LITERAL[value: {}]",
            integer_literal_expr.value()
    );
}

void yy::CodegenVisitor::operator()(const RealLiteralExpr &real_literal_expr) {
    print_ident();
    output_ += std::format(
            "REAL_LITERAL[value: {}]",
            real_literal_expr.value()
    );
}

void yy::CodegenVisitor::operator()(const StringLiteralExpr &string_literal_expr) {
    print_ident();
    output_ += std::format(
            "STRING_LITERAL[value: {}]",
            string_literal_expr.value()
    );
}

void yy::CodegenVisitor::operator()(const ThisExpr &this_expr) {
    print_ident();
    output_ += "THIS";
}

void yy::CodegenVisitor::operator()(const FieldAccessExpr &field_access_expr) {
    print_ident();
    output_ += std::format(
            "FIELD_ACCESS_EXPR[name: {}]",
            field_access_expr.name()
    );
}

void yy::CodegenVisitor::operator()(const MethodCallExpr &method_call_expr) {
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

void yy::CodegenVisitor::operator()(const MemberAccess &member_access) {
    print_ident();
    output_ += "MEMBER_ACCESS";
    depth_++;
    member_access.lhs()->accept(*this);
    member_access.rhs()->accept(*this);
    depth_--;
    print_close_ident();
}

void yy::CodegenVisitor::operator()(const Body &body) {
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

void yy::CodegenVisitor::operator()(const ReturnStmt &return_stmt) {
    print_ident();
    output_ += "RETURN";
    depth_++;
    return_stmt.expression()->accept(*this);
    depth_--;
    print_close_ident();
}

void yy::CodegenVisitor::operator()(const AssignmentStmt &assignment_stmt) {
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

void yy::CodegenVisitor::operator()(const IfStmt &if_stmt) {
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

void yy::CodegenVisitor::operator()(const WhileStmt &while_stmt) {
    print_ident();
    output_ += "WHILE";
    depth_++;
    while_stmt.condition()->accept(*this);
    while_stmt.loop_body()->accept(*this);
    depth_--;
    print_close_ident();
}

void yy::CodegenVisitor::operator()(const MemberDeclaration &member_declaration) {
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

void yy::CodegenVisitor::operator()(const ParameterDeclaration &parameter_declaration) {
    print_ident();
    output_ += std::format(
            "PARAMETER_DECLARATION[name: {}, type: {}]",
            parameter_declaration.name(),
            parameter_declaration.type()
    );
}

void yy::CodegenVisitor::operator()(const VariableDeclaration &variable_declaration) {
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

void yy::CodegenVisitor::operator()(const ConstructorDeclaration &constructor_declaration) {
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

void yy::CodegenVisitor::operator()(const ConstructorDefinition &constructor_definition) {
    print_ident();
    output_ += "CONSTRUCTOR_DEFINITION";
    depth_++;
    constructor_definition.header()->accept(*this);
    constructor_definition.body()->accept(*this);
    depth_--;
    print_close_ident();
}

void yy::CodegenVisitor::operator()(const MethodDeclaration &method_declaration) {
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

void yy::CodegenVisitor::operator()(const MethodDefinition &method_definition) {
    print_ident();
    output_ += "METHOD_DEFINITION";
    depth_++;
    method_definition.header()->accept(*this);
    method_definition.body()->accept(*this);
    depth_--;
    print_close_ident();
}

void yy::CodegenVisitor::operator()(const ProgramDeclaration &program_declaration) {
    yy::RecursiveVisitor<Value *>::operator()(program_declaration);
}


void yy::CodegenVisitor::operator()(const ClassDefinition &class_definition) {
    auto members_ref = class_definition.body()->accept(*this);
    if (members_ref == nullptr || !isa < ArrayRef < Value * >> (members_ref)) {
        result_ = nullptr;
        return;
    }

    auto class_name = class_definition.header()->name();


    llvm::StructType::create(llvm::getGlobalContext(), members_ref, class_name, false);
}

void yy::CodegenVisitor::operator()(const Program &program) {

}


