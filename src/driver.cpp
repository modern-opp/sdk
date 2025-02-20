//
// Created by Nikita Morozov on 20.02.2025.
//

#include "driver.hpp"

yy::driver::driver() : trace_parsing(false) {

}

int yy::driver::parse(const std::string &filename) {
    file = filename;
    scan_begin();
    yy::parser parse(*this);
//    parse.set_debug_level (trace_parsing);
    int res = parse();
    scan_end();
    return res;
}

void yy::driver::scan_begin() {
    location.initialize(&file);
    fin.open(file);

    if (!fin.is_open()) {
        throw std::runtime_error("Cant open file");
    }
}

void yy::driver::scan_end() {
    fin.close();
}
