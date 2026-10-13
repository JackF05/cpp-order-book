#pragma once

#include <cstdint>
#include <list>
#include <unordered_map>
#include "engine/order.hpp"

namespace engine {
    class OrderBook {
        public:
            OrderBook() = default;

            const std::list<Order>& get_bids() const {return bids_;}
            const std::list<Order>& get_asks() const {return asks_;}

            void add_order(const Order& order);
            bool cancel_order(uint64_t order_id);
            bool modify_order(uint64_t order_id, uint64_t new_price, uint64_t new_qty);

        private:
            std::list<Order> bids_;
            std::list<Order> asks_;
            std::unordered_map<uint64_t, std::list<Order>::iterator> order_map_;
    };
}