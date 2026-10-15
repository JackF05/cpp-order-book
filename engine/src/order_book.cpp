#include "engine/order_book.hpp"

namespace engine {
    std::vector<Trade> OrderBook::add_order(Order order) {
        std::vector<Trade> trades;

        if (order.side == Side::Buy) {
            while (!asks_.empty() && order.quantity > 0 && order.price >= asks_.front().price) {
                Order& maker = asks_.front();
                uint64_t trade_qty = std::min(maker.quantity, order.quantity);

                trades.push_back(Trade{maker.id, order.id, trade_qty, maker.price});

                order.quantity -= trade_qty;
                maker.quantity -= trade_qty;

                if (maker.quantity == 0) {
                    order_map_.erase(maker.id);
                    asks_.pop_front();
                }
            }

            if (order.quantity > 0) {
                std::list<Order>::iterator iter = bids_.begin();
                while (iter != bids_.end() && iter->price >= order.price) {
                    ++iter;
                }
                std::list<Order>::iterator inserted_iter = bids_.insert(iter, order);
                order_map_[order.id] = inserted_iter;
            }
        } else {
            while (!bids_.empty() && order.quantity > 0 && order.price <= bids_.front().price) {
                Order& maker = bids_.front();
                uint64_t trade_qty = std::min(maker.quantity, order.quantity);

                trades.push_back(Trade{maker.id, order.id, trade_qty, maker.price});

                order.quantity -= trade_qty;
                maker.quantity -= trade_qty;

                if (maker.quantity == 0) {
                    order_map_.erase(maker.id);
                    bids_.pop_front();
                }
            }

            if (order.quantity > 0) {
                std::list<Order>::iterator iter = asks_.begin();
                while (iter != asks_.end() && iter->price <= order.price) {
                    ++iter;
                }
                std::list<Order>::iterator inserted_iter = asks_.insert(iter, order);
                order_map_[order.id] = inserted_iter;
            }
        }

        return trades;
    }

    bool OrderBook::cancel_order(uint64_t order_id) {
        auto map_iter = order_map_.find(order_id);
        if (map_iter == order_map_.end()) {
            return false;
        }

        auto list_iter = map_iter->second;
        if (list_iter->side == Side::Buy) {
            bids_.erase(list_iter);
        } else {
            asks_.erase(list_iter);
        }

        order_map_.erase(map_iter);

        return true;
    }

    bool OrderBook::modify_order(uint64_t order_id, uint64_t new_price, uint64_t new_qty) {
        auto map_iter = order_map_.find(order_id);
        if (map_iter == order_map_.end()) {
            return false;
        }

        Side side = map_iter->second->side;
        OrderType orderType = map_iter->second->orderType;

        cancel_order(order_id);

        Order updatedOrder{order_id, new_price, new_qty, side, orderType};
        add_order(updatedOrder);

        return true;
    }
}