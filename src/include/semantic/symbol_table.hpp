//
// Created by Nikita Morozov on 25.05.2025.
//

#ifndef OPP_FRONTEND_SYMBOL_TABLE_HPP
#define OPP_FRONTEND_SYMBOL_TABLE_HPP

#include <string>
#include <unordered_map>

#include "semantic/symbol.hpp"

class SymbolTable {
public:

    SymbolTable(SymbolTable *parent = nullptr, std::unique_ptr<Symbol> &&symbol = {});

    SymbolTable *add_symbol(const std::string& name, std::unique_ptr<Symbol> &&symbol);

    SymbolTable * add_child();

    Symbol* getSymbol();

    SymbolTable *resolve_symbol(const std::string &name) const noexcept;

    ClassSymbol *resolve_class(const std::string &name) const noexcept;

    const ClassSymbol *resolve_this() const noexcept;

    InstanceSymbol *resolve_field(
            const std::string &class_name,
            const std::string &field_name
    ) const noexcept;

    MethodSymbol *resolve_method(
            const std::string &class_name,
            const std::string &method_name,
            const std::vector<ClassSymbol*> &args
    ) const noexcept;

    InstanceSymbol *resolve_local(const std::string &name) const noexcept;

    std::string print_debug_info(size_t offset = 0) const;

private:
    std::unique_ptr<Symbol> symbol_;
    std::unordered_map<std::string, std::unique_ptr<SymbolTable>> symbols_;
    SymbolTable *parent_;
    std::vector<std::unique_ptr<SymbolTable>> children_;
};

void register_builtins(SymbolTable* root_table);

#endif //OPP_FRONTEND_SYMBOL_TABLE_HPP
