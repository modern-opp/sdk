//
// Created by Nikita Morozov on 31.03.2025.
//

#ifndef OPP_FRONTEND_AST_UTIL_HPP
#define OPP_FRONTEND_AST_UTIL_HPP


#include "ast.hpp"
#include <string>

namespace yy {
    class PrettyPrintVisitor : public Visitor {
    public:
        void operator()(const NodeBase &node_base) override;

        void operator()(const Expr &expr) override;

        void operator()(const PrimaryExpr &primary_expr) override;

        void operator()(const BooleanLiteralExpr &boolean_literal_expr) override;

        void operator()(const IntegerLiteralExpr &integer_literal_expr) override;

        void operator()(const RealLiteralExpr &real_literal_expr) override;

        void operator()(const StringLiteralExpr &string_literal_expr) override;

        void operator()(const ThisExpr &this_expr) override;

        void operator()(const MemberAccessExpr &member_access_expr) override;

        void operator()(const FieldAccessExpr &field_access_expr) override;

        void operator()(const MethodCallExpr &method_call_expr) override;

        void operator()(const MemberAccess &member_access) override;

        void operator()(const BodyExpr &body_expr) override;

        void operator()(const Body &body) override;

        void operator()(const Stmt &stmt) override;

        void operator()(const ReturnStmt &return_stmt) override;

        void operator()(const AssignmentStmt &assignment_stmt) override;

        void operator()(const IfStmt &if_stmt) override;

        void operator()(const WhileStmt &while_stmt) override;

        void operator()(const MemberDeclarationExpr &member_declaration_expr) override;

        void operator()(const MemberDeclaration &member_declaration) override;

        void operator()(const ParameterDeclaration &parameter_declaration) override;

        void operator()(const VariableDeclaration &variable_declaration) override;

        void operator()(const ConstructorDeclaration &constructor_declaration) override;

        void operator()(const ConstructorDefinition &constructor_definition) override;

        void operator()(const MethodDeclaration &method_declaration) override;

        void operator()(const MethodDefinition &method_definition) override;

        void operator()(const ProgramDeclarationExpr &program_declaration_expr) override;

        void operator()(const ProgramDeclaration &program_declaration) override;

        void operator()(const ClassDeclaration &class_declaration) override;

        void operator()(const ClassDefinition &class_definition) override;

        void operator()(const Program &program) override;

        std::string output() const;

        ~PrettyPrintVisitor() override;

    private:
        void print_ident();

        size_t depth_{};
        std::string output_;
    };
}

#endif //OPP_FRONTEND_AST_UTIL_HPP
