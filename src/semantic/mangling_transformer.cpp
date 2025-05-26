//
// Created by Nikita Morozov on 26.05.2025.
//

#include "semantic/mangling_transformer.hpp"

#include <sstream>

std::string transform_to_mangling_name(
        const std::string &method_name,
        const std::vector<ClassSymbol *> &parameters
) {
    std::ostringstream out;
    out << mangling_delimiter << method_name << mangling_delimiter;

    std::for_each(
            parameters.begin(),
            parameters.end(),
            [&out](auto clazz) {
                out << clazz->name() << mangling_delimiter;
            }
    );

    return out.str();
}