//
// Created by Nikita Morozov on 27.05.2025.
//

#ifndef OPP_FRONTEND_TYPE_CHECKER_VISITOR_HPP
#define OPP_FRONTEND_TYPE_CHECKER_VISITOR_HPP



#include "visitor/recursive_visitor.hpp"
#include "symbol_table.hpp"
#include "symbol_table_index.hpp"
#include "semantic_error.hpp"

namespace yy {

    class TypeCheckerVisitor : public RecursiveVisitor<Symbol*> {
    public:
        TypeCheckerVisitor(
                SymbolTable *scope_symbol_table,
                SymbolTableIndex *symbol_table_index,
                std::vector<SemanticError> &semantic_errors
        ) : RecursiveVisitor(),
            scope_symbol_table_(scope_symbol_table),
            symbol_table_index_(symbol_table_index),
            semantic_errors_(semantic_errors) {}

        void operator()(const BooleanLiteralExpr &boolean_literal_expr) override;

        void operator()(const IntegerLiteralExpr &integer_literal_expr) override;

        void operator()(const RealLiteralExpr &real_literal_expr) override;

        void operator()(const StringLiteralExpr &string_literal_expr) override;

        void operator()(const ThisExpr &this_expr) override;

        void operator()(const FieldAccessExpr &field_access_expr) override;

        void operator()(const MethodCallExpr &method_call_expr) override;

        void operator()(const MemberAccess &member_access) override;

        void operator()(const ReturnStmt &return_stmt) override;

        void operator()(const AssignmentStmt &assignment_stmt) override;

        void operator()(const IfStmt &if_stmt) override;

        void operator()(const WhileStmt &while_stmt) override;

        void operator()(const MemberDeclaration &member_declaration) override;

        void operator()(const ParameterDeclaration &parameter_declaration) override;

        void operator()(const VariableDeclaration &variable_declaration) override;

        void operator()(const ConstructorDeclaration &constructor_declaration) override;

        void operator()(const ConstructorDefinition &constructor_definition) override;

        void operator()(const MethodDeclaration &method_declaration) override;

        void operator()(const MethodDefinition &method_definition) override;

        void operator()(const ProgramDeclaration &program_declaration) override;

        void operator()(const ClassDeclaration &class_declaration) override;

        void operator()(const ClassDefinition &class_definition) override;

        void operator()(const Program &program) override;

        ~TypeCheckerVisitor() override = default;

    private:
        Symbol* callee_ = nullptr;
        SymbolTableIndex *symbol_table_index_;
        std::vector<SemanticError> &semantic_errors_;

        SymbolTable *resolve_method(
                const std::string &method_name,
                const std::vector<std::unique_ptr<ParameterDeclaration>> &parameters
        );
    };
}


#endif //OPP_FRONTEND_TYPE_CHECKER_VISITOR_HPP
