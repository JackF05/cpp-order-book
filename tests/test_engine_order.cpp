#include <gtest/gtest.h>
#include "engine/order.hpp"
#include "engine/order_book.hpp"

using namespace engine;

TEST(OrderBookTest, AddValidBuyOrder) {
    OrderBook book;
    Order order{1, 10, 20, Side::Buy, OrderType::Limit};

    book.add_order(order);

    EXPECT_EQ(book.get_bids().size(), 1);
    EXPECT_EQ(book.get_bids().front().id, 1);
}

TEST(OrderBookTest, OrdersAddedInCorrectOrder) {
    OrderBook book;
    Order order1{1, 10, 20, Side::Buy, OrderType::Limit};
    Order order2{2, 15, 5, Side::Buy, OrderType::Limit};

    book.add_order(order1);
    book.add_order(order2);

    EXPECT_EQ(book.get_bids().size(), 2);
    EXPECT_EQ(book.get_bids().front().id, order2.id);
    EXPECT_EQ(book.get_bids().back().id, order1.id);
}

TEST(OrderBookTest, AddingPreservesTimestampOrderingForSamePries) {
    OrderBook book;
    Order order1{1, 10, 20, Side::Buy, OrderType::Limit};
    Order order2{2, 10, 5, Side::Buy, OrderType::Limit};

    book.add_order(order2);
    book.add_order(order1);

    EXPECT_EQ(book.get_bids().front().id, order2.id);
    EXPECT_EQ(book.get_bids().back().id, order1.id);
}

TEST(OrderBookTest, CancelExistingOrders) {
    OrderBook book;
    Order buy_order{1, 10, 20, Side::Buy, OrderType::Limit};
    Order sell_order{2, 15, 30, Side::Sell, OrderType::Market};

    book.add_order(buy_order);
    bool buy_cancelled = book.cancel_order(buy_order.id);

    EXPECT_EQ(buy_cancelled, true);
    EXPECT_EQ(book.get_bids().size(), 0);

    book.add_order(sell_order);
    bool sell_cancelled = book.cancel_order(sell_order.id);

    EXPECT_EQ(sell_cancelled, true);
    EXPECT_EQ(book.get_asks().size(), 0);
}

TEST(OrderBookTest, CancelsCorrectOrder) {
    OrderBook book;
    Order order1{1, 10, 20, Side::Buy, OrderType::Limit};
    Order order2{2, 15, 30, Side::Buy, OrderType::Limit};
    Order order3{3, 20, 35, Side::Buy, OrderType::Limit};

    book.add_order(order1);
    book.add_order(order2);
    book.add_order(order3);

    bool order_cancelled = book.cancel_order(order2.id);

    EXPECT_EQ(order_cancelled, true);
    EXPECT_EQ(book.get_bids().front().id, order3.id);
    EXPECT_EQ(book.get_bids().back().id, order1.id);
}

TEST(OrderBookTest, CancelWithNoOrders) {
    OrderBook book;

    bool order_cancelled = book.cancel_order(1);

    EXPECT_EQ(order_cancelled, false);
}

TEST(OrderBookTest, NoChangesWithNoCancelledOrderId) {
    OrderBook book;
    Order order1{1, 10, 20, Side::Buy, OrderType::Limit};
    Order order2{2, 15, 30, Side::Buy, OrderType::Limit};

    book.add_order(order1);
    book.add_order(order2);

    bool order_cancelled = book.cancel_order(3);

    EXPECT_EQ(order_cancelled, false);
    EXPECT_EQ(book.get_bids().size(), 2);
}

TEST(OrderBookTest, ModifyOrderCorrectlyModifiesOnlyExistingOrder) {
    OrderBook book;
    Order order{1, 10, 20, Side::Buy, OrderType::Limit};

    book.add_order(order);
    bool modified_order = book.modify_order(order.id, 20, 15);

    EXPECT_EQ(modified_order, true);
    EXPECT_EQ(book.get_bids().size(), 1);
    EXPECT_EQ(book.get_bids().front().id, 1);
    EXPECT_EQ(book.get_bids().front().price, 20);
    EXPECT_EQ(book.get_bids().front().quantity, 15);
}

TEST(OrderBookTest, ModifyOrderMovesPosition) {
    OrderBook book;
    Order order1{1, 10, 20, Side::Buy, OrderType::Limit};
    Order order2{2, 10, 20, Side::Buy, OrderType::Limit};

    book.add_order(order1);
    book.add_order(order2);

    EXPECT_EQ(book.get_bids().front().id, order1.id);

    bool modified_order = book.modify_order(order2.id, 15, 20);

    EXPECT_EQ(modified_order, true);
    EXPECT_EQ(book.get_bids().front().id, order2.id);
}

TEST(OrderBookTest, FailedModifyDoesNotChangeBook) {
    OrderBook book;
    Order order1{1, 10, 20, Side::Buy, OrderType::Limit};
    Order order2{2, 10, 20, Side::Buy, OrderType::Limit};

    book.add_order(order1);
    book.add_order(order2);

    EXPECT_EQ(book.get_bids().front().id, order1.id);
    EXPECT_EQ(book.get_bids().back().id, order2.id);

    bool modified_order = book.modify_order(3, 15, 20);

    EXPECT_EQ(modified_order, false);
    EXPECT_EQ(book.get_bids().front().id, order1.id);
    EXPECT_EQ(book.get_bids().back().id, order2.id);
}

TEST(OrderBookTest, PartialFillTrade) {
    OrderBook book;
    Order maker{1, 10, 20, Side::Buy, OrderType::Limit};
    Order taker{2, 5, 15, Side::Sell, OrderType::Limit};

    auto empty_trades = book.add_order(maker);
    auto placed_trades = book.add_order(taker);

    ASSERT_EQ(empty_trades.size(), 0);
    ASSERT_EQ(placed_trades.size(), 1);
    ASSERT_EQ(book.get_bids().size(), 1);
    ASSERT_EQ(book.get_asks().size(), 0);
}

TEST(OrderBookTest, MultiLevelSweepWithResidualRest) {
    OrderBook book;
    Order maker1{1, 10, 5, Side::Buy, OrderType::Limit};
    Order maker2{2, 12, 10, Side::Buy, OrderType::Limit};
    Order taker{3, 8, 20, Side::Sell, OrderType::Limit};

    auto empty_trades1 = book.add_order(maker1);
    auto empty_trades2 = book.add_order(maker2);
    auto placed_trades = book.add_order(taker);

    ASSERT_EQ(empty_trades1.size(), 0);
    ASSERT_EQ(empty_trades2.size(), 0);
    ASSERT_EQ(placed_trades.size(), 2);
    ASSERT_EQ(book.get_bids().size(), 0);
    ASSERT_EQ(book.get_asks().size(), 1);

    ASSERT_EQ(placed_trades[0].price, 12);
    ASSERT_EQ(placed_trades[0].quantity, 10);
    
    ASSERT_EQ(placed_trades[1].price, 10);
    ASSERT_EQ(placed_trades[1].quantity, 5);
}