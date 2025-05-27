//
// Created by Nikita Morozov on 25.05.2025.
//

#ifndef OPP_FRONTEND_SYMBOL_TABLE_INDEX_HPP
#define OPP_FRONTEND_SYMBOL_TABLE_INDEX_HPP

#include <unordered_map>
#include "semantic/symbol_table.hpp"


class SymbolTableIndex {
public:
    SymbolTableIndex();

    void commit(const NodeBase *location, SymbolTable *symbol_table);

    SymbolTable &restore(const NodeBase *location);

private:
    std::unordered_map<const NodeBase *, SymbolTable *> index_;
};

#endif //OPP_FRONTEND_SYMBOL_TABLE_INDEX_HPP
