//
// Created by Nikita Morozov on 27.05.2025.
//

#ifndef OPP_FRONTEND_SEMANTIC_ERROR_HPP
#define OPP_FRONTEND_SEMANTIC_ERROR_HPP

#include <string>
#include <ostream>
#include "parser/location.hh"

class SemanticError {
public:
    SemanticError(
            const std::string &message,
            const yy::location &location,
            bool fatal = false
    ) : message_(message), location_(location), fatal_(fatal) {}

    const std::string &message() const {
        return message_;
    }

    const yy::location &location() const {
        return location_;
    }

    bool fatal() const {
        return fatal_;
    }

    friend std::ostream &operator<<(std::ostream &os, const SemanticError &error) {
        if (error.fatal_) {
            os << "FATAL ";
        }

        os << "ERROR:" << error.message_ << " at location: " << error.location_;
        return os;
    }

private:
    bool fatal_;
    std::string message_;
    yy::location location_;
};

#endif //OPP_FRONTEND_SEMANTIC_ERROR_HPP
