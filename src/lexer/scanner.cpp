//
// Created by Nikita Morozov on 27.03.2025.
//

#include "scanner.hpp"

#include <utility>
#include "buffered_reader.hpp"

yy::Scanner::Scanner(
        yy::BufferedReader reader,
        const std::__1::basic_string<char> &filename
) : reader_(std::move(reader)),
    filename_(filename) {}

char yy::Scanner::peek() const {
    return reader_.peek();
}

char yy::Scanner::advance() {
    char ch = reader_.peek();
    if (ch == '\n') {
        lines_ += 1;
        lineOffset_ = 0;
    } else {
        lineOffset_++;
    }

    return reader_.advance();
}

const std::string &yy::Scanner::content() const {
    return reader_.content();
}

std::string yy::Scanner::substring(size_t start, size_t delta) const {
    return reader_.substring(start, delta);
}

bool yy::Scanner::eof() const {
    return reader_.eof();
}

void yy::Scanner::begin_token() {
    begin_ = yy::position(&filename_, lines_, lineOffset_);
}

yy::location yy::Scanner::end_token() {
    yy::position end{&filename_, lines_, lineOffset_};
    return {{begin_}, end};
}
