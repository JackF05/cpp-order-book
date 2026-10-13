#include "engine/order_book.hpp"
#include "engine/order.hpp"

#include <list>

namespace engine {
    void OrderBook::add_order(const Order& order) {
        if (order.side == Side::Buy) {
            std::list<Order>::iterator iter = bids_.begin();
            while (iter != bids_.end() && iter->price >= order.price) {
                ++iter;
            }
            std::list<Order>::iterator inserted_iter = bids_.insert(iter, order);
            order_map_[order.id] = inserted_iter;
        } else {
            std::list<Order>::iterator iter = asks_.begin();
            while (iter != asks_.end() && iter->price <= order.price) {
                ++iter;
            }
            std::list<Order>::iterator inserted_iter = asks_.insert(iter, order);
            order_map_[order.id] = inserted_iter;
        }
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