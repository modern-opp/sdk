//
// Created by Nikita Morozov on 27.05.2025.
//

#include "semantic/cfa_visitor.hpp"




void yy::CFAVisitor::operator()(const ReturnStmt &return_stmt) {
    result_ = 1;
}


void yy::CFAVisitor::operator()(const IfStmt &if_stmt) {
    size_t prev = result_;
    size_t then = if_stmt.then_body()->accept(*this);
    if (if_stmt.else_body()) {
        size_t elze = if_stmt.else_body()->accept(*this);
        if (then & elze) {
            result_ = 1;
            return;
        }
    }
    result_ = prev;
}


void yy::CFAVisitor::operator()(const ConstructorDefinition &constructor_definition) {

}


void yy::CFAVisitor::operator()(const MethodDefinition &method_definition) {
    result_ = 0;
    size_t res = method_definition.body()->accept(*this);
    if (res != 1 && !method_definition.header()->return_type().empty()) {
        semantic_errors_.emplace_back(
                "Missing return statement",
                method_definition.location()
        );
    }
}




