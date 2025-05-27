//
// Created by Nikita Morozov on 27.05.2025.
//

#ifndef OPP_FRONTEND_SIMPLE_VISITOR_HPP
#define OPP_FRONTEND_SIMPLE_VISITOR_HPP

#include "ast/ast.hpp"
#include <string>

namespace yy {

    class SimpleVisitor : public Visitor<void> {
    public:
        SimpleVisitor() {

        }

        void operator()(const BooleanLiteralExpr &boolean_literal_expr)   override {}

        void operator()(const IntegerLiteralExpr &integer_literal_expr)  override {}

        void operator()(const RealLiteralExpr &real_literal_expr)  override {}

        void operator()(const StringLiteralExpr &string_literal_expr)  override {}

        void operator()(const ThisExpr &this_expr)  override {}

        void operator()(const FieldAccessExpr &field_access_expr)  override {}

        void operator()(const MethodCallExpr &method_call_expr)  override {}

        void operator()(const MemberAccess &member_access)  override {}

        void operator()(const Body &body)  override {}

        void operator()(const ReturnStmt &return_stmt)  override {}

        void operator()(const AssignmentStmt &assignment_stmt)  override {}

        void operator()(const IfStmt &if_stmt)  override {}

        void operator()(const WhileStmt &while_stmt)  override {}

        void operator()(const MemberDeclaration &member_declaration)  override {}

        void operator()(const ParameterDeclaration &parameter_declaration)  override {}

        void operator()(const VariableDeclaration &variable_declaration)  override {}

        void operator()(const ConstructorDeclaration &constructor_declaration)  override {}

        void operator()(const ConstructorDefinition &constructor_definition)  override {}

        void operator()(const MethodDeclaration &method_declaration)  override {}

        void operator()(const MethodDefinition &method_definition)  override {}

        void operator()(const ProgramDeclaration &program_declaration)  override {}

        void operator()(const ClassDeclaration &class_declaration)  override {}

        void operator()(const ClassDefinition &class_definition)  override {}

        void operator()(const Program &program)  override {}
    };
}

#endif //OPP_FRONTEND_SIMPLE_VISITOR_HPP
