//
// Created by Nikita Morozov on 20.02.2025.
//

#include "driver.hpp"
#include "lexer/scanner.hpp"
#include "lexer/buffered_reader.hpp"
#include "visitor/pretty_print_visitor.hpp"
#include "semantic/symbol_table.hpp"
#include "semantic/symbol_table_class_collector_visitor.hpp"
#include "semantic/symbol_table_method_collector_visitor.hpp"
#include "semantic/symbol_table_visitor.hpp"
#include "semantic/symbol_table_index.hpp"
#include "semantic/semantic_error.hpp"

yy::driver::driver() {

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

    std::cout << ":: BEGIN TOKEN SEQUENCE ::" << std::endl << std::endl;
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


    std::cout << ":: BEGIN SEMANTIC ANALYSIS  ::" << std::endl << std::endl;
    std::vector<SemanticError> semantic_errors;
    auto symbol_table_index = std::make_unique<SymbolTableIndex>();
    auto symbol_table = std::make_unique<SymbolTable>(nullptr, std::unique_ptr<Symbol>());
    register_builtins(symbol_table.get());

    auto top_table_visitor = SymbolTableClassCollectorVisitor(symbol_table.get(), semantic_errors);
    program->accept(top_table_visitor);

    auto membre_visitor = SymbolTableMethodCollectorVisitor(symbol_table.get(), semantic_errors);
    program->accept(membre_visitor);

    auto symbols_visitor = SymbolTableVisitor(symbol_table.get(), symbol_table_index.get(), semantic_errors);
    program->accept(symbols_visitor);

    std::cout << symbol_table->print_debug_info() << std::endl;

    std::for_each(semantic_errors.begin(), semantic_errors.end(), [](auto &error) {
        std::cerr << error << std::endl;
    });

    std::cout << ":: END SEMANTIC ANALYSIS  ::" << std::endl << std::endl;

    return 0;
}
