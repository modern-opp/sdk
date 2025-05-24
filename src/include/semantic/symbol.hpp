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
    const std::string& name() const noexcept;

    NodeBase *declaredNode() const noexcept;

protected:
    Symbol(const std::string &name, NodeBase *declaredNode);

private:
    std::string name_;
    NodeBase *declaredNode_;
};

class ClassSymbol;


class InstanceSymbol : Symbol {
public:
    InstanceSymbol(ClassSymbol *clazz, const std::string &name, NodeBase *declaredNode);

    ClassSymbol *clazz() const noexcept;

private:
    ClassSymbol *clazz_;
};

class MethodSymbol : Symbol {
public:
    MethodSymbol(
            ClassSymbol *clazz,
            std::vector<InstanceSymbol *> parameters,
            ClassSymbol *returnType,
            const std::string &name,
            NodeBase *declaredNode
    );

    const std::vector<InstanceSymbol *> &parameters() const noexcept;

    ClassSymbol *returnType() const noexcept;

    ClassSymbol *clazz() const noexcept;

private:
    ClassSymbol *clazz_;
    std::vector<InstanceSymbol *> parameters_;
    ClassSymbol *returnType_;
};

class ClassSymbol : Symbol {
public:
    ClassSymbol(const std::string &name, NodeBase *declaredNode);
};


#endif //OPP_FRONTEND_SYMBOL_HPP
