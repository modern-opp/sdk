//
// Created by Nikita Morozov on 27.05.2025.
//

#ifndef OPP_FRONTEND_ENTRYPOINT_VISITOR_HPP
#define OPP_FRONTEND_ENTRYPOINT_VISITOR_HPP

#include "ast/ast.hpp"
#include "visitor/simple_visitor.hpp"
#include "symbol_table.hpp"
#include "symbol_table_index.hpp"
#include "semantic_error.hpp"
#include <string>

namespace yy {

    class EntryPointVisitor : public SimpleVisitor {
    public:

        EntryPointVisitor(SymbolTableIndex *symbolTableIndex, std::vector<SemanticError> &semanticErrors)
                : symbol_table_index_(symbolTableIndex), semantic_errors_(semanticErrors) {}

        void operator()(const Program &program) override {
            auto constructor_chain = dynamic_cast<const MemberAccess *>(program.main_class());

            if (!constructor_chain) {
                expect_constructor_call(program);
                return;
            }

            auto this_call = dynamic_cast<const ThisExpr *>(constructor_chain->lhs());

            if (!this_call) {
                expect_constructor_call(program);
                return;
            }

            auto method_call = dynamic_cast<const MethodCallExpr *>(constructor_chain->rhs());

            if (!method_call) {
                expect_constructor_call(program);
                return;
            }

            auto &symbol_table = symbol_table_index_->restore(&program);
            auto main_class = symbol_table.resolve_class(method_call->name());

            if (!main_class) {
                expect_constructor_call(program);
            }

        }


    private:
        SymbolTableIndex *symbol_table_index_;
        std::vector<SemanticError> &semantic_errors_;

        void expect_constructor_call(const Program &program) {
            semantic_errors_.emplace_back(
                    "Main class constructor expected",
                    program.main_class()->location()
            );
        }
    };
}
#endif //OPP_FRONTEND_ENTRYPOINT_VISITOR_HPP
