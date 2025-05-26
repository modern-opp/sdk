//
// Created by Nikita Morozov on 25.05.2025.
//

#ifndef OPP_FRONTEND_SYMBOL_HPP
#define OPP_FRONTEND_SYMBOL_HPP

#include "ast/ast.hpp"
#include <string>
#include <vector>

class Symbol {
public:
    const std::string &name() const noexcept;

    virtual const NodeBase *declaredNode() const noexcept;

    virtual std::string print_debug_info(size_t offset) const = 0;

protected:
    Symbol(const std::string &name, const NodeBase *declaredNode);

private:
    std::string name_;
    const NodeBase *declaredNode_;
};

class ClassSymbol;


class InstanceSymbol : public Symbol {
public:
    InstanceSymbol(const ClassSymbol *clazz, const std::string &name, const NodeBase *declaredNode);

    const ClassSymbol *clazz() const noexcept;

    std::string print_debug_info(size_t offset = 0) const override;

private:
    const ClassSymbol *clazz_;
};

enum MethodSymbolKind {
    method_declaration,
    method_definition,
    constructor_declaration,
    constructor_definition
};

class MethodSymbol : public Symbol {
public:
    MethodSymbol(
            MethodSymbolKind kind,
            const ClassSymbol *clazz,
            std::vector<ClassSymbol *> &&parameters,
            const ClassSymbol *returnType,
            const std::string &name,
            const NodeBase *declaredNode
    );

    MethodSymbolKind kind() const;

    const MethodDefinition *declaredNode() const noexcept override;

    const std::vector<ClassSymbol *> &parameters() const noexcept;

    const ClassSymbol *returnType() const noexcept;

    const ClassSymbol *clazz() const noexcept;

    std::string print_debug_info(size_t offset = 0) const override;

private:
    MethodSymbolKind kind_;
    const ClassSymbol *clazz_;
    std::vector<ClassSymbol *> parameters_;
    const ClassSymbol *returnType_;
};

class ClassSymbol : public Symbol {
public:
    ClassSymbol(const std::string &name, const NodeBase *declaredNode);

    const ClassDefinition *declaredNode() const noexcept override;

    std::string print_debug_info(size_t offset = 0) const override;
};


#endif //OPP_FRONTEND_SYMBOL_HPP
