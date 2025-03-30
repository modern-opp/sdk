//
// Created by Nikita Morozov on 27.03.2025.
//

#ifndef OPP_FRONTEND_SCANNER_HPP
#define OPP_FRONTEND_SCANNER_HPP

#include "parser.tab.hpp"
#include "buffered_reader.hpp"

namespace yy {

    class Scanner {
    public:
        Scanner(BufferedReader reader, const std::string &filename);

        char peek() const;

        char advance();

        const std::string &content() const;

        std::string substring(size_t start, size_t delta) const;

        bool eof() const;

        void begin_token();

        yy::location end_token();

    private:
        yy::position begin_{};
        int lines_ = 0;
        int lineOffset_ = 0;
        std::string filename_;
        BufferedReader reader_;
    };
};

#endif //OPP_FRONTEND_SCANNER_HPP
