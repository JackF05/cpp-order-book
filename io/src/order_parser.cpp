#include "io/order_parser.hpp"
#include <sstream>
#include <algorithm>

namespace io {
    std::optional<engine::Order> parse_order(std::istream& in, uint64_t& next_id) {
        std::string line;

        if (!std::getline(in, line)) {
            // no line given
            return std::nullopt;
        }

        std::stringstream ss(line);
        std::string side_str;
        uint64_t price;
        uint64_t quantity;

        if (!(ss >> side_str >> price >> quantity)) {
            // cant parse line properly
            return std::nullopt;
        }

        std::transform(side_str.begin(), side_str.end(), side_str.begin(), [](unsigned char c){ return std::tolower(c); });

        engine::Side side;
        if (side_str == "buy") {
            side = engine::Side::Buy;
        } else if (side_str == "sell") {
            side = engine::Side::Sell;
        } else {
            return std::nullopt;
        }

        return engine::Order{next_id++, price, quantity, side, engine::OrderType::Limit};
    }
}