#include <gtest/gtest.h>
#include <sstream>
#include <iostream>
#include "io/order_parser.hpp"

TEST(OrderParserTest, StandardBuyWorks) {
    std::istringstream input("BUY 100 10");
    uint64_t next_id = 1;

    std::optional<engine::Order> order = io::parse_order(input, next_id);

    ASSERT_TRUE(order.has_value());
    
    EXPECT_EQ(order->side, engine::Side::Buy);
    EXPECT_EQ(order->price, 100);
    EXPECT_EQ(order->quantity, 10);
}

TEST(OrderParserTest, StandardSellWorks) {
    std::istringstream input("SELL 100 10");
    uint64_t next_id = 1;

    std::optional<engine::Order> order = io::parse_order(input, next_id);

    ASSERT_TRUE(order.has_value());
    
    EXPECT_EQ(order->side, engine::Side::Sell);
    EXPECT_EQ(order->price, 100);
    EXPECT_EQ(order->quantity, 10);
}

TEST(OrderParserTest, MixedCasingWorks) {
    std::istringstream input("bUy 100 10");
    uint64_t next_id = 1;

    std::optional<engine::Order> order = io::parse_order(input, next_id);

    ASSERT_TRUE(order.has_value());
    
    EXPECT_EQ(order->side, engine::Side::Buy);
    EXPECT_EQ(order->price, 100);
    EXPECT_EQ(order->quantity, 10);
}

TEST(OrderParserTest, IrregularWhiteSpaceWorks) {
    std::istringstream input("BUY     100  10");
    uint64_t next_id = 1;

    std::optional<engine::Order> order = io::parse_order(input, next_id);

    ASSERT_TRUE(order.has_value());
    
    EXPECT_EQ(order->side, engine::Side::Buy);
    EXPECT_EQ(order->price, 100);
    EXPECT_EQ(order->quantity, 10);
}

TEST(OrderParserTest, EmptyLineFails) {
    std::istringstream input("\nBUY 100 10");
    uint64_t next_id = 1;

    std::optional<engine::Order> order = io::parse_order(input, next_id);

    ASSERT_FALSE(order.has_value());
}

TEST(OrderParserTest, CommentLineFails) {
    std::istringstream input("# BUY 100 10");
    uint64_t next_id = 1;

    std::optional<engine::Order> order = io::parse_order(input, next_id);

    ASSERT_FALSE(order.has_value());
}

TEST(OrderParserTest, InvalidSideFails) {
    std::istringstream input("INVALID_SIDE 100 10");
    uint64_t next_id = 1;

    std::optional<engine::Order> order = io::parse_order(input, next_id);

    ASSERT_FALSE(order.has_value());
}

TEST(OrderParserTest, InvalidPriceFails) {
    std::istringstream input("BUY 1,000 10");
    uint64_t next_id = 1;

    std::optional<engine::Order> order = io::parse_order(input, next_id);

    ASSERT_FALSE(order.has_value());
}

TEST(OrderParserTest, InvalidQuantityFails) {
    std::istringstream input("BUY 100 thirty");
    uint64_t next_id = 1;

    std::optional<engine::Order> order = io::parse_order(input, next_id);

    ASSERT_FALSE(order.has_value());
}

TEST(OrderParserTest, IncompleteFieldsFail) {
    std::istringstream input("BUY 100");
    uint64_t next_id = 1;

    std::optional<engine::Order> order = io::parse_order(input, next_id);

    ASSERT_FALSE(order.has_value());
}

TEST(OrderParserTest, MultipleOrdersWork) {
    std::istringstream input("BUY 100 10\nSELL 105 5");
    uint64_t next_id = 1;

    std::optional<engine::Order> buy_order = io::parse_order(input, next_id);

    ASSERT_TRUE(buy_order.has_value());
    
    EXPECT_EQ(buy_order->side, engine::Side::Buy);
    EXPECT_EQ(buy_order->price, 100);
    EXPECT_EQ(buy_order->quantity, 10);

    std::optional<engine::Order> sell_order = io::parse_order(input, next_id);

    ASSERT_TRUE(sell_order.has_value());
    
    EXPECT_EQ(sell_order->side, engine::Side::Sell);
    EXPECT_EQ(sell_order->price, 105);
    EXPECT_EQ(sell_order->quantity, 5);
}