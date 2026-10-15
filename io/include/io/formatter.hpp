#pragma once

#include <vector>
#include "engine/order_book.hpp"
#include "engine/trade.hpp"

namespace io {
    void print_trades(const std::vector<engine::Trade>& trades);
    void print_book(const engine::OrderBook& book);
}