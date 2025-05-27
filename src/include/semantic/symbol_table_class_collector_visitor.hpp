//
// Created by Nikita Morozov on 26.05.2025.
//

#ifndef SDK_SYMBOL_TABLE_CLASS_COLLECTOR_VISITOR_HPP
#define SDK_SYMBOL_TABLE_CLASS_COLLECTOR_VISITOR_HPP

#include "semantic/symbol_table.hpp"
#include "visitor/recursive_visitor.hpp"
#include "semantic_error.hpp"


namespace yy {

    class SymbolTableClassCollectorVisitor : public RecursiveVisitor<SymbolTable *> {
    public:
        SymbolTableClassCollectorVisitor(
                SymbolTable *scope_symbol_table,
                std::vector<SemanticError> &semantic_errors
        )
                : RecursiveVisitor(),
                  scope_symbol_table_(scope_symbol_table),
                  semantic_errors_(semantic_errors) {
        }

        void operator()(const ClassDefinition &class_definition) override;

        ~SymbolTableClassCollectorVisitor() override = default;

    private:
        SymbolTable *scope_symbol_table_;
        std::vector<SemanticError> &semantic_errors_;
    };
}


#endif //SDK_SYMBOL_TABLE_CLASS_COLLECTOR_VISITOR_HPP
