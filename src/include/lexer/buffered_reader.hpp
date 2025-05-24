//
// Created by Nikita Morozov on 27.03.2025.
//

#ifndef OPP_FRONTEND_BUFFERED_READER_HPP
#define OPP_FRONTEND_BUFFERED_READER_HPP

#include <string>
#include <fstream>
#include <cstddef>

namespace yy {
    class BufferedReader {
    public:
        constexpr static char nPos = -1;

        explicit BufferedReader(std::string buffer);

        size_t offset() const;

        char peek() const;

        char advance();

        const std::string &content() const;

        std::string substring(size_t start, size_t delta) const;

        bool eof() const;

    private:
        size_t offset_ = 0;
        std::string buffer_{};
    };
}


#endif //OPP_FRONTEND_BUFFERED_READER_HPP
