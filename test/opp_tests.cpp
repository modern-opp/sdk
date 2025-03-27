//
// Created by Nikita Morozov on 27.03.2025.
//

#include <gtest/gtest.h>
#include <string>
#include <filesystem>
#include <iostream>
#include "driver.hpp"

class OppFrontendTests : public testing::TestWithParam<std::string> {
};

namespace {
    std::vector<std::string> GetTests() {
        const std::string path_string = PATH_GOOD_TEST;
        std::cout << "Discover test" << path_string << std::endl;
        const std::filesystem::path path(path_string);

        std::vector<std::string> res;

        for (const auto &file: std::filesystem::directory_iterator(path)) {
            res.emplace_back(path_string + file.path().string());
        }
        return res;
    }
}

TEST_P(OppFrontendTests, GoodTests) {
    const auto path = GetParam();
    std::cout << "Discover test" << path << std::endl;

    yy::driver driver;
    driver.parse(path);
}

INSTANTIATE_TEST_SUITE_P(Container, OppFrontendTests, testing::ValuesIn(GetTests()));

