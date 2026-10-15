#pragma once

#include <iostream>
#include <optional>
#include <string>
#include "engine/order.hpp"
#include "engine/order_book.hpp"

namespace io {
    std::optional<engine::Order> parse_order(std::istream& in, uint64_t& next_id);
}