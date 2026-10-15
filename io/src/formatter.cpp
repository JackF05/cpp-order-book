#include "io/formatter.hpp"
#include <iostream>

namespace io {
    void print_trades(const std::vector<engine::Trade>& trades) {
        std::cout << "TRADES MADE:\n";
        for (const auto& trade : trades) {
            std::cout << "  Maker " << trade.maker_order_id
                << " and Taker " << trade.taker_order_id
                << " traded " << trade.quantity
                << " for $" << trade.price << "\n";
        }
    }

    void print_book(const engine::OrderBook& book) {
        std::cout << "ORDER BOOK STATE\n";
        std::cout << "BIDS:\n";
        for (const auto& order : book.get_bids()) {
            std::cout << "  ID: " << order.id
            << " | Price: " << order.price
            << " | Quantity: " << order.quantity << "\n";
        }

        std::cout << "ASKS:\n";
        for (const auto& order : book.get_asks()) {
            std::cout << "  ID: " << order.id
            << " | Price: " << order.price
            << " | Quantity: " << order.quantity << "\n";
        }
    }
}