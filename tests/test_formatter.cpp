#include <gtest/gtest.h>
#include <sstream>
#include <iostream>
#include "io/formatter.hpp"
#include "engine/trade.hpp"

TEST(FormatterTest, FormatsTradesCorrectly) {
    std::vector<engine::Trade> trades = {
        {1, 2, 100, 10}
    };

    std::stringstream buffer;
    std::streambuf* old_cout = std::cout.rdbuf(buffer.rdbuf());

    io::print_trades(trades);

    std::cout.rdbuf(old_cout);

    std::string output = buffer.str();

    EXPECT_NE(output.find("100"), std::string::npos);
    EXPECT_NE(output.find("10"), std::string::npos);
}