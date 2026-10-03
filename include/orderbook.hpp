#pragma once

#include <list>
#include <map>
#include <memory>
#include <queue>
#include <unordered_map>
#include "order.hpp"
#include "pricelevel.hpp"

/*
 * We need a CancelStatus because a cancel order failing is not a failure in
 * the code, if someone tries to cancel their order but the matching engine
 * matches the order first, then the cancel order must fail gracefully
 */
enum class CancelStatus
{
    Success,
    OverFill,
    Fail,
};

enum class ModifyStatus
{
    Success,
    Fail,
};



// orderbook class that holds the asks and bids, also performs the order
// matching
class OrderBook
{
public:
    OrderBook(const size_t expectedOrders = 0)
        : asks_{}
          , bids_{}
          , orders_{}
    {
        orders_.reserve(expectedOrders);
    }

    void AddOrder(const OrderPointer &order);

    CancelStatus CancelOrder(OrderId orderID, Quantity quantity);

    ModifyStatus ModifyOrder(OrderId orderID, Price price, Quantity quantity);

    void MatchOrders();

    Price GetBestBid() const;

    Price GetBestAsk() const;

private:
    // TODO: experiment with different data structures and convert Order to
    // pointers
    std::map<Price, PriceLevel, std::less<Price> > asks_; // highest ask at top
    std::map<Price, PriceLevel, std::greater<Price> > bids_; // lowest bid at top
    // keep track of orders by ID, to get their location and quickly modify
    // this is to reduce latency for future Cancel and Modify functions
    std::unordered_map<OrderId, std::shared_ptr<OrderEntry>> orders_;

    bool CanMatch(Side side, Price price);

    template <typename T>
    void AddOrderToSide(T& side, const OrderPointer& order);
};


