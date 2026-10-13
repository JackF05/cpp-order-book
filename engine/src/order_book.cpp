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
            bids_.insert(iter, order);
        } else {
            std::list<Order>::iterator iter = asks_.begin();
            while (iter != asks_.end() && iter->price <= order.price) {
                ++iter;
            }
            asks_.insert(iter, order);
        }
    }

    bool OrderBook::cancel_order(uint64_t order_id) {
        for (std::list<Order>::iterator iter = bids_.begin(); iter != bids_.end(); ++iter) {
            if (iter->id == order_id) {
                bids_.erase(iter);
                return true;
            }
        }
        
        for (std::list<Order>::iterator iter = asks_.begin(); iter != asks_.end(); ++iter) {
            if (iter->id == order_id) {
                asks_.erase(iter);
                return true;
            }
        }

        return false;
    }

    bool OrderBook::modify_order(uint64_t order_id, uint64_t new_price, uint64_t new_qty) {
        for (std::list<Order>::iterator iter = bids_.begin(); iter != bids_.end(); ++iter) {
            if (iter->id == order_id) {
                Order updated_order{order_id, new_price, new_qty, iter->side, iter->orderType};
                bids_.erase(iter);
                add_order(updated_order);
                return true;
            }
        }
        
        for (std::list<Order>::iterator iter = asks_.begin(); iter != asks_.end(); ++iter) {
            if (iter->id == order_id) {
                Order updated_order{order_id, new_price, new_qty, iter->side, iter->orderType};
                asks_.erase(iter);
                add_order(updated_order);
                return true;
            }
        }

        return false;
    }
}