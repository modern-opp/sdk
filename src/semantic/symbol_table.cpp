//
// Created by Nikita Morozov on 25.05.2025.
//

#include <sstream>

#include "semantic/symbol_table.hpp"
#include "semantic/mangling_transformer.hpp"


SymbolTable::SymbolTable(SymbolTable *parent, std::unique_ptr<Symbol> &&symbol)
        : parent_(parent), symbol_(std::move(symbol)), symbols_{}, children_() {

}

Symbol *SymbolTable::getSymbol() {
    return symbol_.get();
}

SymbolTable *SymbolTable::add_symbol(const std::string &name, std::unique_ptr<Symbol> &&symbol) {
    if (auto it = symbols_.find(name); it != symbols_.end()) {
        return nullptr;
    }

    auto child = std::make_unique<SymbolTable>(this, std::move(symbol));
    auto it = symbols_.emplace(name, std::move(child));
    if (!it.second) {
        return nullptr;
    }

    return it.first->second.get();
}

SymbolTable *SymbolTable::add_child() {
    auto child = std::make_unique<SymbolTable>();

    children_.push_back(std::move(child));
    return children_.rbegin()->get();
}

SymbolTable *SymbolTable::resolve_symbol(const std::string &name) const noexcept {
    if (symbol_ && symbol_->name() == name) {
        return const_cast<SymbolTable *>(this);
    }

    if (auto symbol = symbols_.find(name); symbol != symbols_.end()) {
        return symbol->second.get();
    } else {
        return parent_ ? parent_->resolve_symbol(name) : nullptr;
    }
}

const ClassSymbol *SymbolTable::resolve_this() const noexcept {
    for (auto symbol_table = this;
         symbol_table->symbol_ != nullptr;
         symbol_table = symbol_table->parent_) {
        if (auto clazz = dynamic_cast<const ClassSymbol *>(symbol_table->symbol_.get()); clazz != nullptr) {
            return clazz;
        }
    }

    return nullptr;
}

ClassSymbol *SymbolTable::resolve_class(const std::string &name) const noexcept {
    SymbolTable *child = resolve_symbol(name);
    if (!child) {
        return nullptr;
    }

    return dynamic_cast<ClassSymbol *>(child->symbol_.get());
}

MethodSymbol *SymbolTable::resolve_method(
        const std::string &class_name,
        const std::string &method_name,
        const std::vector<ClassSymbol *> &args
) const noexcept {
    auto clazz_table = resolve_symbol(class_name);
    if (!clazz_table) {
        return nullptr;
    }
    auto name = transform_to_mangling_name(method_name, args);
    auto method_table = clazz_table->resolve_symbol(method_name);
    return dynamic_cast<MethodSymbol *>(method_table->symbol_.get());
}

InstanceSymbol *SymbolTable::resolve_field(
        const std::string &class_name,
        const std::string &field_name
) const noexcept {
    auto clazz_table = resolve_symbol(class_name);
    if (!clazz_table) {
        return nullptr;
    }

    return clazz_table->resolve_local(field_name);
}

InstanceSymbol *SymbolTable::resolve_local(const std::string &name) const noexcept {
    SymbolTable *local_table = resolve_symbol(name);
    if (!local_table) {
        return nullptr;
    }

    return dynamic_cast<InstanceSymbol *>(local_table->symbol_.get());
}

std::string SymbolTable::print_debug_info(size_t offset) const {
    std::ostringstream out;

    out << "{" << std::endl;
    out << std::string(offset, ' ') << R"( "type": "symbol_table")" << std::endl;
    if (symbol_) {
        out << std::string(offset, ' ') << " \"symbol\": " << symbol_->print_debug_info(offset + 1) << std::endl;
    }
    if (!symbols_.empty()) {
        out << std::string(offset, ' ') << " \"symbols\": [" << std::endl;
        std::for_each(symbols_.begin(), symbols_.end(), [&out, &offset](auto &pair) {
            out << std::string(offset + 1, ' ') << "\"" << pair.first << "\": ";
            out << pair.second->print_debug_info(offset + 1);
        });
        out << std::string(offset, ' ') << "]" << std::endl;
    }
    if (!children_.empty()) {
        out << std::string(offset, ' ') << " \"children\": [" << std::endl;
        std::for_each(children_.begin(), children_.end(), [&out, &offset](auto &child) {
            out << child->print_debug_info(offset + 1);
        });
        out << std::string(offset, ' ') << "]" << std::endl;
    }
    out << std::string(offset, ' ') << "}";

    return out.str();
}

void register_builtins(SymbolTable *root_table) {
    std::string string_class = "String";
    std::string integer_class = "Integer";
    std::string real_class = "Real";
    std::string bool_class = "Boolean";

    root_table->add_symbol(string_class, std::make_unique<ClassSymbol>(string_class, nullptr));
    root_table->add_symbol(integer_class, std::make_unique<ClassSymbol>(integer_class, nullptr));
    root_table->add_symbol(real_class, std::make_unique<ClassSymbol>(real_class, nullptr));
    root_table->add_symbol(bool_class, std::make_unique<ClassSymbol>(bool_class, nullptr));
}