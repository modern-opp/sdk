//
// Created by Nikita Morozov on 20.02.2025.
//

#include "parser.tab.hpp"
#include "driver.hpp"

int main(int argc, char **argv) {
    yy::driver driver;
    std::string filename{argv[1]};
    driver.parse(filename);
    return 0;
}
