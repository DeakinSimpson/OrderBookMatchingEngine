#pragma once

#include <cstdint>
#include <iostream>
#include <list>
#include <map>
#include <memory>
#include <unordered_map>
#include <vector>

enum class Side {
  Bid,
  Ask,
  None,
};

enum class TradeType {
  Add,
  Cancel,
  Modify,
  None,
};

using OrderId = uint64_t;
using Price = double;
using Quantity = uint32_t;  // cant have negative stock

class Order
{
public:
  Order(const OrderId id, const Side side, Price const price,
    const Quantity quantity)
      : id_{ id }
      , side_{ side }
      , price_{ price }
      , quantity_{ quantity }
  {  };

  // [[nodiscard]] means that we cant call this as a no return function
  [[nodiscard]] OrderId GetId() const { return id_; }
  [[nodiscard]] Side GetSide() const { return side_; }
  [[nodiscard]] Price GetPrice() const { return price_; }
  [[nodiscard]] Quantity GetQuantity() const { return quantity_; }

  void Fill(Quantity quantity);

private:
  OrderId id_;
  Side side_;
  Price price_;
  Quantity quantity_;
};

/*
 * Original thought was to use std::vector as it has better cache locality,
 * and O(1) random access whereas list has O(n), however because vector
 * invalidates iterators where there is a insertion or deletion i have chosen
 * to use list instead
 */
using OrderPointer = std::shared_ptr<Order>;
using OrderPointers = std::list<OrderPointer>;

/*
 * We need a CancelStatus because a cancel order failing is not a failure in
 * the code, if someone tries to cancel their order but the matching engine
 * matches the order first, then the cancel order must fail gracefully
 */
enum class CancelStatus {
  Success,
  OverFill,
  Fail,
};

// orderbook class that holds the asks and bids, also performs the order
// matching
class OrderBook
{
public:
  OrderBook() 
      : asks_{}
      , bids_{}
      , orders_{}
  {  }
  
  void AddOrder(const OrderPointer& order);
  CancelStatus CancelOrder(OrderId orderID, Quantity quantity);
  void MatchOrders();

  Price GetBestBid() const;
  Price GetBestAsk() const;

private:
  struct OrderEntry {
    OrderPointer order_ {};
    OrderPointers::iterator iterator_;
  };

  // TODO: experiment with different data structures and convert Order to
  // pointers
  std::map<Price, OrderPointers, std::less<Price>> asks_;  // highest ask at top
  std::map<Price, OrderPointers, std::greater<Price>> bids_;     // lowest bid at top
  // keep track of orders by ID, to get their location and quickly modify
  // this is to reduce latency for future Cancel and Modify functions
  std::unordered_map<OrderId, std::shared_ptr<OrderEntry>> orders_;

  bool CanMatch(Side side, Price price);
};

struct TradeInfo
{
  OrderId orderID;
  Side side;
  Price price;
  Quantity quantity;
  TradeType tradeType;
};

class Trade
{
  TradeInfo tradeInfo_;

public:
  explicit Trade(const TradeInfo& tradeInfo) : tradeInfo_{tradeInfo} {  }
  [[nodiscard]] TradeInfo GetTradeInfo() const { return tradeInfo_; }
  [[nodiscard]] OrderPointer ToOrderPointer() const;

  void MakeTrade(OrderBook& orderBook) const;
};

