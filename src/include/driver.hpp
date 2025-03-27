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

        void scan_begin();

        void scan_end();

        std::string file{};
        std::ifstream fin{};
        bool trace_parsing;
        yy::location location;
    };
}


#endif //OPP_FRONTEND_DRIVER_HPP
