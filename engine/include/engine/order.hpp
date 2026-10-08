#pragma once

#include <cstdint>

namespace engine {
    enum class Side: uint8_t {Buy , Sell};
    enum class OrderType: uint8_t {Limit, Market};

    struct Order {
        uint64_t id;
        uint64_t price;
        uint64_t quantity;
        Side side;
        OrderType orderType;
    };

    inline Order create_order(uint64_t id, uint64_t price, uint64_t quantity, Side side, OrderType orderType) {
        return Order{id, price, quantity, side, orderType};
    }
}