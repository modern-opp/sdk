//
// Created by Nikita Morozov on 26.05.2025.
//

#ifndef OPP_FRONTEND_MANGLING_TRANSFORMER_HPP
#define OPP_FRONTEND_MANGLING_TRANSFORMER_HPP

#include <string>
#include "symbol.hpp"

constexpr std::string mangling_delimiter = "$$";

std::string transform_to_mangling_name(
        const std::string &method_name,
        const std::vector<ClassSymbol *> &parameters
);

#endif //OPP_FRONTEND_MANGLING_TRANSFORMER_HPP
