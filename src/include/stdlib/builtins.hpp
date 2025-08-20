//
// Created by Nikita Morozov on 27.05.2025.
//

#ifndef OPP_FRONTEND_BUILTINS_HPP
#define OPP_FRONTEND_BUILTINS_HPP

#include "semantic/symbol_table.hpp"
#include <string>

namespace oppstd {

    struct IntegerNames {
        static constexpr std::string kClass = "Integer";
        static constexpr std::string kMin = "Min";
        static constexpr std::string kMax = "Max";
        static constexpr std::string kToReal = "toReal";
        static constexpr std::string kToBoolean = "toBoolean";
        static constexpr std::string kUnaryMinus = "UnaryMinus";
        static constexpr std::string kPlus = "Plus";
        static constexpr std::string kMinus = "Minus";
        static constexpr std::string kMult = "Mult";
        static constexpr std::string kDiv = "Div";
        static constexpr std::string kRem = "Rem";
        static constexpr std::string kLess = "Less";
        static constexpr std::string kLessEqual = "LessEqual";
        static constexpr std::string kGreater = "Greater";
        static constexpr std::string kGreaterEqual = "GreaterEqual";
        static constexpr std::string kEqual = "Equal";
    };

    struct RealNames {
        static constexpr std::string kClass = "Real";
        static constexpr std::string kMin = "Min";
        static constexpr std::string kMax = "Max";
        static constexpr std::string kEpsilon = "Epsilon";
        static constexpr std::string kToInteger = "toInteger";
        static constexpr std::string kUnaryMinus = "UnaryMinus";
        static constexpr std::string kPlus = "Plus";
        static constexpr std::string kMinus = "Minus";
        static constexpr std::string kMult = "Mult";
        static constexpr std::string kDiv = "Div";
        static constexpr std::string kRem = "Rem";
        static constexpr std::string kLess = "Less";
        static constexpr std::string kLessEqual = "LessEqual";
        static constexpr std::string kGreater = "Greater";
        static constexpr std::string kGreaterEqual = "GreaterEqual";
        static constexpr std::string kEqual = "Equal";
    };

    struct BooleanNames {
        static constexpr std::string kClass = "Boolean";
        static constexpr std::string kToInteger = "toInteger";
        static constexpr std::string kOr = "Or";
        static constexpr std::string kAnd = "And";
        static constexpr std::string kXor = "Xor";
    };

    struct StringNames {
        static constexpr std::string kClass = "String";
    };


    void register_builtins(SymbolTable* root_table);
}



#endif //OPP_FRONTEND_BUILTINS_HPP
