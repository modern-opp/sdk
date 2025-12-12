//
// Created by Nikita Morozov on 27.05.2025.
//

#include "stdlib/builtins.hpp"
#include "semantic/mangling_transformer.hpp"

namespace oppstd {

    // Common comparison operations for numeric and string types
    static const std::vector<std::string> comparison_ops = {"Less", "Greater", "LessEqual", "GreaterEqual", "Equal", "NotEqual", "Equals"};

    static void register_integer_methods(SymbolTable *root_table, ClassSymbol *integer_sym, ClassSymbol *bool_sym) {
        auto integer_table = root_table->resolve_symbol(oppstd::integer_class);
        
        // Binary arithmetic operations: Integer -> Integer
        std::vector<std::string> binary_int_ops = {"Plus", "Minus", "Mult", "Div", "Mod"};
        for (const auto& op : binary_int_ops) {
            std::vector<ClassSymbol*> params = {integer_sym};
            auto name = transform_to_mangling_name(op, params);
            auto symbol = std::make_unique<MethodSymbol>(
                method_declaration,
                integer_sym,
                std::vector<ClassSymbol*>{integer_sym},
                integer_sym,
                op,
                nullptr
            );
            integer_table->add_symbol(name, std::move(symbol));
        }
        
        // Comparison operations: Integer -> Boolean
        for (const auto& op : comparison_ops) {
            std::vector<ClassSymbol*> params = {integer_sym};
            auto name = transform_to_mangling_name(op, params);
            auto symbol = std::make_unique<MethodSymbol>(
                method_declaration,
                integer_sym,
                std::vector<ClassSymbol*>{integer_sym},
                bool_sym,
                op,
                nullptr
            );
            integer_table->add_symbol(name, std::move(symbol));
        }
        
        // Unary operations
        {
            std::vector<ClassSymbol*> params = {};
            auto name = transform_to_mangling_name("Negate", params);
            auto symbol = std::make_unique<MethodSymbol>(
                method_declaration,
                integer_sym,
                std::vector<ClassSymbol*>{},
                integer_sym,
                "Negate",
                nullptr
            );
            integer_table->add_symbol(name, std::move(symbol));
        }
    }

    static void register_real_methods(SymbolTable *root_table, ClassSymbol *real_sym, ClassSymbol *bool_sym) {
        auto real_table = root_table->resolve_symbol(oppstd::real_class);
        
        // Binary arithmetic operations: Real -> Real
        std::vector<std::string> binary_real_ops = {"Plus", "Minus", "Mult", "Div"};
        for (const auto& op : binary_real_ops) {
            std::vector<ClassSymbol*> params = {real_sym};
            auto name = transform_to_mangling_name(op, params);
            auto symbol = std::make_unique<MethodSymbol>(
                method_declaration,
                real_sym,
                std::vector<ClassSymbol*>{real_sym},
                real_sym,
                op,
                nullptr
            );
            real_table->add_symbol(name, std::move(symbol));
        }
        
        // Comparison operations: Real -> Boolean
        for (const auto& op : comparison_ops) {
            std::vector<ClassSymbol*> params = {real_sym};
            auto name = transform_to_mangling_name(op, params);
            auto symbol = std::make_unique<MethodSymbol>(
                method_declaration,
                real_sym,
                std::vector<ClassSymbol*>{real_sym},
                bool_sym,
                op,
                nullptr
            );
            real_table->add_symbol(name, std::move(symbol));
        }
        
        // Unary operations
        {
            std::vector<ClassSymbol*> params = {};
            auto name = transform_to_mangling_name("Negate", params);
            auto symbol = std::make_unique<MethodSymbol>(
                method_declaration,
                real_sym,
                std::vector<ClassSymbol*>{},
                real_sym,
                "Negate",
                nullptr
            );
            real_table->add_symbol(name, std::move(symbol));
        }
    }

    static void register_boolean_methods(SymbolTable *root_table, ClassSymbol *bool_sym) {
        auto bool_table = root_table->resolve_symbol(oppstd::bool_class);
        
        // Logical operations: Boolean -> Boolean
        std::vector<std::string> binary_bool_ops = {"And", "Or", "Equal", "NotEqual", "Equals"};
        for (const auto& op : binary_bool_ops) {
            std::vector<ClassSymbol*> params = {bool_sym};
            auto name = transform_to_mangling_name(op, params);
            auto symbol = std::make_unique<MethodSymbol>(
                method_declaration,
                bool_sym,
                std::vector<ClassSymbol*>{bool_sym},
                bool_sym,
                op,
                nullptr
            );
            bool_table->add_symbol(name, std::move(symbol));
        }
        
        // Unary operations
        {
            std::vector<ClassSymbol*> params = {};
            auto name = transform_to_mangling_name("Not", params);
            auto symbol = std::make_unique<MethodSymbol>(
                method_declaration,
                bool_sym,
                std::vector<ClassSymbol*>{},
                bool_sym,
                "Not",
                nullptr
            );
            bool_table->add_symbol(name, std::move(symbol));
        }
    }

    static void register_string_methods(SymbolTable *root_table, ClassSymbol *string_sym, ClassSymbol *integer_sym, ClassSymbol *bool_sym) {
        auto string_table = root_table->resolve_symbol(oppstd::string_class);
        
        // String concatenation: String -> String
        {
            std::vector<ClassSymbol*> params = {string_sym};
            auto name = transform_to_mangling_name("Concat", params);
            auto symbol = std::make_unique<MethodSymbol>(
                method_declaration,
                string_sym,
                std::vector<ClassSymbol*>{string_sym},
                string_sym,
                "Concat",
                nullptr
            );
            string_table->add_symbol(name, std::move(symbol));
        }
        
        // String length: () -> Integer
        {
            std::vector<ClassSymbol*> params = {};
            auto name = transform_to_mangling_name("Length", params);
            auto symbol = std::make_unique<MethodSymbol>(
                method_declaration,
                string_sym,
                std::vector<ClassSymbol*>{},
                integer_sym,
                "Length",
                nullptr
            );
            string_table->add_symbol(name, std::move(symbol));
        }
        
        // String comparison: String -> Boolean
        for (const auto& op : comparison_ops) {
            std::vector<ClassSymbol*> params = {string_sym};
            auto name = transform_to_mangling_name(op, params);
            auto symbol = std::make_unique<MethodSymbol>(
                method_declaration,
                string_sym,
                std::vector<ClassSymbol*>{string_sym},
                bool_sym,
                op,
                nullptr
            );
            string_table->add_symbol(name, std::move(symbol));
        }
    }

    void register_builtins(SymbolTable *root_table) {
        // Create class symbols
        root_table->add_symbol(string_class, std::make_unique<ClassSymbol>(string_class, nullptr));
        root_table->add_symbol(integer_class, std::make_unique<ClassSymbol>(integer_class, nullptr));
        root_table->add_symbol(real_class, std::make_unique<ClassSymbol>(real_class, nullptr));
        root_table->add_symbol(bool_class, std::make_unique<ClassSymbol>(bool_class, nullptr));
        
        // Get pointers to the class symbols
        auto integer_sym = root_table->resolve_class(integer_class);
        auto real_sym = root_table->resolve_class(real_class);
        auto bool_sym = root_table->resolve_class(bool_class);
        auto string_sym = root_table->resolve_class(string_class);
        
        // Register methods for each class
        register_integer_methods(root_table, integer_sym, bool_sym);
        register_real_methods(root_table, real_sym, bool_sym);
        register_boolean_methods(root_table, bool_sym);
        register_string_methods(root_table, string_sym, integer_sym, bool_sym);
    }
}