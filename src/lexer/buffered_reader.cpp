//
// Created by Nikita Morozov on 27.03.2025.
//

#include "buffered_reader.hpp"

#include <utility>

namespace yy {

    BufferedReader::BufferedReader(std::string buffer) : buffer_(std::move(buffer)) {

    }

    size_t BufferedReader::offset() const {
        return offset_ - 1;
    }

    char BufferedReader::peek() const {
        if (offset_ >= buffer_.size()) {
            return -1;
        }
        return buffer_[offset_];
    }

    char BufferedReader::advance() {
        if (offset_ >= buffer_.size()) {
            return -1;
        }

        return buffer_[offset_++];
    }

    const std::string &BufferedReader::content() const {
        return buffer_;
    }

    std::string BufferedReader::substring(size_t start, size_t delta) const {
        return buffer_.substr(start, offset_ + delta);
    }

    bool BufferedReader::eof() const {
        return offset_ > buffer_.size() - 1;
    }
}