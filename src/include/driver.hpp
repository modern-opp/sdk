//
// Created by Nikita Morozov on 20.02.2025.
//

#ifndef OPP_FRONTEND_DRIVER_HPP
#define OPP_FRONTEND_DRIVER_HPP

#include <string>
#include <fstream>
#include <map>
#include "parser.tab.hpp"

namespace yy {
    class driver {
    public:
        driver();

        int parse(const std::string &filename);

    private:
        std::string file{};
        bool trace_parsing;
        yy::location location;
    };
}


#endif //OPP_FRONTEND_DRIVER_HPP
