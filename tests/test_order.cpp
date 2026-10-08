#include <gtest/gtest.h>
#include "engine/order.hpp"
#include <iostream>

using namespace engine;

TEST(OrderTest, Instantiation) {
    Order order{1, 10, 20, Side::Buy, OrderType::Limit};

    EXPECT_EQ(order.id, 1);
    EXPECT_EQ(order.price, 10);
    EXPECT_EQ(order.quantity, 20);
    EXPECT_EQ(order.side, Side::Buy);
    EXPECT_EQ(order.orderType, OrderType::Limit);
};