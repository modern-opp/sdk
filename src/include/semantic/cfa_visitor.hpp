//
// Created by Nikita Morozov on 27.05.2025.
//

#ifndef SDK_CFA_VISITOR_HPP
#define SDK_CFA_VISITOR_HPP


#include "visitor/recursive_visitor.hpp"
#include "symbol_table.hpp"
#include "symbol_table_index.hpp"
#include "semantic_error.hpp"

namespace yy {

    class CFAVisitor : public RecursiveVisitor<size_t> {
    public:
        CFAVisitor(
                std::vector<SemanticError> &semantic_errors
        ) : RecursiveVisitor(),
            semantic_errors_(semantic_errors) {}

        void operator()(const ReturnStmt &return_stmt) override;

        void operator()(const IfStmt &if_stmt) override;

        void operator()(const ConstructorDefinition &constructor_definition) override;

        void operator()(const MethodDefinition &method_definition) override;

        ~CFAVisitor() override = default;

    private:
        std::vector<SemanticError> &semantic_errors_;
    };
}


#endif //SDK_CFA_VISITOR_HPP
