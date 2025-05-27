//
// Created by Nikita Morozov on 25.05.2025.
//

#include "semantic/symbol_table_index.hpp"

SymbolTableIndex::SymbolTableIndex() : index_{} {}

void SymbolTableIndex::commit(const NodeBase *location, SymbolTable *symbol_table) {
    index_.emplace(location, symbol_table);
}

SymbolTable &SymbolTableIndex::restore(const NodeBase *location) {
    return const_cast<SymbolTable &>(*index_.find(location)->second);
}
