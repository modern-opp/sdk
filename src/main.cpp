//
// Created by Nikita Morozov on 20.02.2025.
//

#include "parser.tab.hpp"
#include "driver.hpp"

int main() {
    yy::driver driver;
    driver.parse("test.opp");
    return 0;
}
