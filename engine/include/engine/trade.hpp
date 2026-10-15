#pragma once

#include <cstdint>

namespace engine {
    struct Trade {
        uint64_t maker_order_id;
        uint64_t taker_order_id;
        uint64_t quantity;
        uint64_t price;
    };
}