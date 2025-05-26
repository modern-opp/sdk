//
// Created by Nikita Morozov on 31.03.2025.
//

#ifndef OPP_FRONTEND_RECURSIVE_VISITOR_HPP
#define OPP_FRONTEND_RECURSIVE_VISITOR_HPP


#include "ast/ast.hpp"
#include <string>

namespace yy {
    template<class T>
    class RecursiveVisitor : public Visitor<T> {
    public:
        RecursiveVisitor() {}

        void operator()(const BooleanLiteralExpr &boolean_literal_expr) override {

        }

        void operator()(const IntegerLiteralExpr &integer_literal_expr) override {};

        void operator()(const RealLiteralExpr &real_literal_expr) override {};

        void operator()(const StringLiteralExpr &string_literal_expr) override {};

        void operator()(const ThisExpr &this_expr) override {};

        void operator()(const FieldAccessExpr &field_access_expr) override {};

        void operator()(const MethodCallExpr &method_call_expr) override {
            std::for_each(
                    method_call_expr.arguments().begin(),
                    method_call_expr.arguments().end(),
                    [this](auto &ptr) {
                        ptr->accept(*this);
                    }
            );
        }


        void operator()(const MemberAccess &member_access) override {
            member_access.lhs()->accept(*this);
            member_access.rhs()->accept(*this);
        }

        void operator()(const Body &body) override {
            std::for_each(
                    body.expressions().begin(),
                    body.expressions().end(),
                    [this](auto &ptr) {
                        ptr->accept(*this);
                    }
            );
        }

        void operator()(const ReturnStmt &return_stmt) override {
            return_stmt.expression()->accept(*this);
        }

        void operator()(const AssignmentStmt &assignment_stmt) override {
            assignment_stmt.expression()->accept(*this);
        }

        void operator()(const IfStmt &if_stmt) override {
            if_stmt.condition()->accept(*this);
            if_stmt.then_body()->accept(*this);
            if (if_stmt.else_body()) {
                if_stmt.else_body()->accept(*this);
            }
        }

        void operator()(const WhileStmt &while_stmt) override {
            while_stmt.condition()->accept(*this);
            while_stmt.loop_body()->accept(*this);
        }


        void operator()(const MemberDeclaration &member_declaration) override {
            std::for_each(
                    member_declaration.member_declarations().begin(),
                    member_declaration.member_declarations().end(),
                    [this](auto &ptr) {
                        ptr->accept(*this);
                    }
            );
        }


        void operator()(const ParameterDeclaration &parameter_declaration) override {}

        void operator()(const VariableDeclaration &variable_declaration) override {
            variable_declaration.initializer()->accept(*this);
        }

        void operator()(const ConstructorDeclaration &constructor_declaration) override {
            std::for_each(
                    constructor_declaration.parameters().begin(),
                    constructor_declaration.parameters().end(),
                    [this](auto &ptr) {
                        ptr->accept(*this);
                    }
            );
        }

        void operator()(const ConstructorDefinition &constructor_definition) override {
            constructor_definition.header()->accept(*this);
            constructor_definition.body()->accept(*this);
        }

        void operator()(const MethodDeclaration &method_declaration) override {
            std::for_each(
                    method_declaration.parameters().begin(),
                    method_declaration.parameters().end(),
                    [this](auto &ptr) {
                        ptr->accept(*this);
                    }
            );
        }

        void operator()(const MethodDefinition &method_definition) override {
            method_definition.header()->accept(*this);
            method_definition.body()->accept(*this);
        }

        void operator()(const ProgramDeclaration &program_declaration) override {
            std::for_each(
                    program_declaration.class_declarations().begin(),
                    program_declaration.class_declarations().end(),
                    [this](auto &ptr) {
                        ptr->accept(*this);
                    }
            );
        }

        void operator()(const ClassDeclaration &class_declaration) override {}

        void operator()(const ClassDefinition &class_definition) override {
            class_definition.header()->accept(*this);
            class_definition.body()->accept(*this);
        }

        void operator()(const Program &program) override {
            program.class_declarations()->accept(*this);
            program.main_class()->accept(*this);
        }

        ~RecursiveVisitor() override {

        };
    };
}

#endif //OPP_FRONTEND_RECURSIVE_VISITOR_HPP
