//
// Created by Nikita Morozov on 25.05.2025.
//

#include "semantic/symbol.hpp"

Symbol::Symbol(const std::string& name, NodeBase *declaredNode) : name_(name), declaredNode_(declaredNode) {}

const std::string& Symbol::name() const noexcept {
    return name_;
}

NodeBase *Symbol::declaredNode() const noexcept {
    return declaredNode_;
}

InstanceSymbol::InstanceSymbol(ClassSymbol* clazz, const std::string& name, NodeBase *declaredNode) :
        Symbol(name, declaredNode),
        clazz_(clazz) {}

ClassSymbol *InstanceSymbol::clazz() const noexcept {
    return clazz_;
}

MethodSymbol::MethodSymbol(
        ClassSymbol* clazz,
        std::vector<InstanceSymbol *> parameters,
        ClassSymbol *returnType,
        const std::string& name,
        NodeBase *declaredNode
) : Symbol(name, declaredNode),
    clazz_(clazz),
    parameters_(parameters),
    returnType_(returnType) {

}

ClassSymbol::ClassSymbol(const std::string& name, NodeBase *declaredNode) : Symbol(name, declaredNode) {}
