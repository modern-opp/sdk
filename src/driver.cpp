//
// Created by Nikita Morozov on 20.02.2025.
//

#include "driver.hpp"
#include "lexer/scanner.hpp"
#include "lexer/buffered_reader.hpp"
#include "visitor/pretty_print_visitor.hpp"

yy::driver::driver()  {

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
    yy::Scanner scanner{BufferedReader(read_file(filename)), filename};
    std::unique_ptr<Program> program;

    yy::parser parse(scanner, program);

    std::cout << ":: BEGIN TOKEN SEQUENCE ::" << std::endl<< std::endl;
    int parse_failure = parse();
    std::cout << ":: END TOKEN SEQUENCE ::" << std::endl << std::endl;

    if (parse_failure) {
        std::cerr << ":: ERROR DURING PARSING ::" << std::endl;
        return parse_failure;
    }

    auto pretty_printer = yy::PrettyPrintVisitor();
    program->accept(pretty_printer);
    std::cout << ":: BEGIN DUMP AST NODES  ::" << std::endl << std::endl;
    std::cout << pretty_printer.output() << std::endl;
    std::cout << ":: END DUMP AST NODES  ::" << std::endl << std::endl;

    return 0;
}
