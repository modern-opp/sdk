//
// Created by Nikita Morozov on 26.05.2025.
//

#ifndef OPP_FRONTEND_SYMBOL_TABLE_METHOD_COLLECTOR_VISITOR_HPP
#define OPP_FRONTEND_SYMBOL_TABLE_METHOD_COLLECTOR_VISITOR_HPP

#include "semantic/symbol_table.hpp"
#include "visitor/recursive_visitor.hpp"

namespace yy {

    class SymbolTableMethodCollectorVisitor : public RecursiveVisitor<SymbolTable *> {
    public:
        SymbolTableMethodCollectorVisitor(SymbolTable *scope_symbol_table)
                : RecursiveVisitor(), scope_symbol_table_(scope_symbol_table) {
        }

        void operator()(const VariableDeclaration &variable_declaration) override;

        void operator()(const ConstructorDefinition &constructor_definition) override;

        void operator()(const ConstructorDeclaration &constructor_declaration) override;

        void operator()(const MethodDeclaration &method_declaration) override;

        void operator()(const MethodDefinition &method_definition) override;

        void operator()(const ClassDeclaration &class_declaration) override;

        void operator()(const ClassDefinition &class_definition) override;

        ~SymbolTableMethodCollectorVisitor() override = default;

    private:
        SymbolTable *scope_symbol_table_;

        void add_method(
                MethodSymbolKind kind,
                const std::string &method_name,
                const std::vector<std::unique_ptr<ParameterDeclaration>> &parameters,
                const ClassSymbol *return_type,
                const NodeBase *node
        );
    };
}

#endif //OPP_FRONTEND_SYMBOL_TABLE_METHOD_COLLECTOR_VISITOR_HPP
