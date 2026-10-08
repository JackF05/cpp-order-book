#include "engine/order.hpp"
#include <iostream>

int main() {
    engine::Order order = engine::create_order(1, 10, 20, engine::Side::Buy, engine::OrderType::Limit);
    std::cout << "Order created " << order.id << "\n";
    return 0;
}