#include "engine/order_book.hpp"
#include "io/formatter.hpp"
#include "io/order_parser.hpp"
#include <iostream>
#include <fstream>

int main(int argc, char* argv[]) {
    engine::OrderBook book;
    uint64_t next_id = 1;

    std::istream* input = &std::cin;
    std::ifstream file;

    if (argc > 1) {
        file.open(argv[1]);

        if (!file.is_open()) {
            std::cerr << "Failed to open file " << argv[1] << "\n";
            return 1;
        }

        input = &file;
        std::cout << "Reading orders from " << argv[1] << "\n";
    } else {
        std::cout << "Reading orders from stdin (BUY/SELL <price> <qty>)\n";
    }

    while (auto order = io::parse_order(*input, next_id)) {
        std::cout << "[ORDER] ID: " << order->id
        << " (" << (order->side == engine::Side::Buy ? "BUY" : "SELL")
        << " " << order->quantity << " @ " << order->price << ")\n";

        auto trades = book.add_order(*order);

        io::print_trades(trades);
    }

    io::print_book(book);
    
    return 0;
}