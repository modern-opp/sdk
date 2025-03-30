//
// Created by Nikita Morozov on 20.02.2025.
//

#include "driver.hpp"
#include "scanner.hpp"
#include "buffered_reader.hpp"

yy::driver::driver() : trace_parsing(false) {

}

static std::string read_file(const std::string &filename) {
    std::ifstream fin{};
    fin.open(filename);

    if (!fin.is_open()) {
        throw std::runtime_error("Cant open file: " + filename);
    }

    return {
            std::istreambuf_iterator<char>(fin),
            std::istreambuf_iterator<char>()
    };
}

int yy::driver::parse(const std::string &filename) {
    file = filename;
    yy::Scanner scanner{BufferedReader(read_file(filename)), filename};

    yy::parser parse(scanner);
    int res = parse();
    return res;
}
