//
// Created by Nikita Morozov on 27.05.2025.
//

#ifndef OPP_FRONTEND_INHERITANCE_VISITOR_HPP
#define OPP_FRONTEND_INHERITANCE_VISITOR_HPP


#include "visitor/recursive_visitor.hpp"
#include "symbol_table.hpp"
#include "symbol_table_index.hpp"
#include "semantic_error.hpp"

namespace yy {

    class InheritanceVisitor : public RecursiveVisitor<Symbol *> {
    public:
        InheritanceVisitor(
                SymbolTableIndex *symbol_table_index,
                std::vector<SemanticError> &semantic_errors
        ) : RecursiveVisitor(),
            symbol_table_index_(symbol_table_index),
            semantic_errors_(semantic_errors) {}

        void operator()(const VariableDeclaration &variable_declaration) override;

        void operator()(const ClassDeclaration &class_declaration) override;

        ~InheritanceVisitor() override = default;

    private:
        SymbolTableIndex *symbol_table_index_;
        std::vector<SemanticError> &semantic_errors_;
    };
}


#endif //OPP_FRONTEND_INHERITANCE_VISITOR_HPP
