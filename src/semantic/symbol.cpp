//
// Created by Nikita Morozov on 25.05.2025.
//

#include "semantic/symbol.hpp"

#include <sstream>

Symbol::Symbol(const std::string &name, const NodeBase *declaredNode) : name_(name), declaredNode_(declaredNode) {}

const std::string &Symbol::name() const noexcept {
    return name_;
}

const NodeBase *Symbol::declaredNode() const noexcept {
    return declaredNode_;
}


InstanceSymbol::InstanceSymbol(
        InstanceSymbolKind kind,
        const ClassSymbol *clazz,
        const std::string &name,
        const NodeBase *declaredNode
) :
        Symbol(name, declaredNode),
        clazz_(clazz),
        kind_(kind) {}

const ClassSymbol *InstanceSymbol::clazz() const noexcept {
    return clazz_;
}

std::string InstanceSymbol::print_debug_info(size_t offset) const {
    std::ostringstream out;

    out << "{" << std::endl;
    out << std::string(offset, ' ') << R"( "type": "instance")" << std::endl;
    out << std::string(offset, ' ') << " \"name\": " << name() << std::endl;
    out << std::string(offset, ' ') << " \"kind\": " << kind() << std::endl;
    if (clazz()) {
        out << std::string(offset, ' ') << " \"clazzID\": " << clazz()->name() << std::endl;
    }
    out << std::string(offset, ' ') << "}";

    return out.str();
}

InstanceSymbolKind InstanceSymbol::kind() const {
    return kind_;
}

void InstanceSymbol::setClazz(const ClassSymbol *clazz) {
    clazz_ = clazz;
}

MethodSymbol::MethodSymbol(
        MethodSymbolKind kind,
        const ClassSymbol *clazz,
        std::vector<ClassSymbol *> &&parameters,
        const ClassSymbol *returnType,
        const std::string &name,
        const NodeBase *declaredNode
) : Symbol(name, declaredNode),
    kind_(kind),
    clazz_(clazz),
    parameters_(std::move(parameters)),
    returnType_(returnType) {

}

const std::vector<ClassSymbol *> &MethodSymbol::parameters() const noexcept {
    return parameters_;
}

const ClassSymbol *MethodSymbol::returnType() const

noexcept {
    return
            returnType_;
}

const ClassSymbol *MethodSymbol::clazz() const

noexcept {
    return
            clazz_;
}


const MethodDefinition *MethodSymbol::declaredNode() const

noexcept {
    return dynamic_cast

            <const MethodDefinition *> (Symbol::declaredNode());

}


std::string MethodSymbol::print_debug_info(size_t offset) const {
    std::ostringstream out;

    out << "{" << std::endl;
    out << std::string(offset, ' ') << R"( "type": "method")" << std::endl;
    out << std::string(offset, ' ') << " \"name\": " << name() << std::endl;
    if (returnType()) {
        out << std::string(offset, ' ') << " \"return\": " << returnType()->name() << std::endl;
    }
    out << std::string(offset, ' ') << " \"kind\": " << kind() << std::endl;
    if (!parameters().empty()) {
        out << std::string(offset, ' ') << " \"params\": [" << std::endl;
        std::for_each(parameters().begin(), parameters().end(), [&out, &offset](auto param) {
            out << param->print_debug_info(offset + 2) << "," << std::endl;
        });
    }
    out << std::string(offset, ' ') << " ]" << std::endl;
    out << std::string(offset, ' ') << "},";

    return out.str();
}

MethodSymbolKind MethodSymbol::kind() const {
    return kind_;
}


ClassSymbol::ClassSymbol(const std::string &name, const NodeBase *declaredNode) : Symbol(name, declaredNode) {}


const ClassDefinition *ClassSymbol::declaredNode() const noexcept {
    return dynamic_cast<const ClassDefinition *> (Symbol::declaredNode());
}


std::string ClassSymbol::print_debug_info(size_t offset) const {
    std::ostringstream out;

    out << "{" << std::endl;
    out << std::string(offset, ' ') << R"( "type": "clazz")" << std::endl;
    out << std::string(offset, ' ') << " \"name\": " << name() << std::endl;
    out << std::string(offset, ' ') << "}";

    return out.str();
}


