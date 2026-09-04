#pragma once

#include <cstdint>
#include <iostream>
#include <map>
#include <vector>

enum class Side
{
  Bid,
  Ask,
  None,
};

enum class TradeType
{
  Add,
  Cancel,
  Modify,
  None,
};

using OrderId = uint64_t;
using Price = int64_t;    
using Quantity = uint32_t;  // cant have negative stock

class Order
{
public:
  Order(OrderId id, Side side, Price price,
    Quantity quantity)
      : id_{ id }
      , side_{ side }
      , price_{ price }
      , quantity_{ quantity }
  {  };

  OrderId GetId() { return id_; }
  Side GetSide() { return side_; }
  Price GetPrice() { return price_; }
  Quantity GetQuantity() { return quantity_; }
  
  // TODO: add check for overfill
  void Fill(Quantity quantity) 
  { 
    if (quantity > quantity_) {
      std::cerr << "Cant Fill Order for More than its Quantity" << std::endl;
    }
    quantity_ -= quantity; 
  }

private:
  OrderId id_;
  Side side_;
  Price price_;
  Quantity quantity_;
};

using Orders = std::vector<Order>;

// orderbook class that holds the asks and bids, also performs the order
// matching
class OrderBook
{
public:
  OrderBook() 
      : asks_{}
      , bids_{}
  {  }
  
  void AddOrder(Order order) 
  {
    if (order.GetSide() == Side::Ask) {
      // get the orders for that price
      auto& orders { asks_[order.GetPrice()] };
      orders.push_back(order);

    } else {
      auto& orders { bids_[order.GetPrice()] };
      orders.push_back(order);
    }
  }

  void MatchOrders() {
  // loop through each bid and ask Price
    while (true) {
      // if there are no bids or asks then there is nothing to match
      if (bids_.empty() || asks_.empty())
        { break; }

      const auto& askPriceLevel { asks_.begin() };
      const auto& bidPriceLevel { bids_.begin() };

      // if the best ask is higher then the best bid no orders can match
      if (bidPriceLevel->first > askPriceLevel->first)
        { break; }

      // loop through each bid and ask and try to match at this level
      while (askPriceLevel->second.size() && bidPriceLevel->second.size()) {
        // FIFO, get fist value of vector
        auto& bid { bidPriceLevel->second.front() };
        auto& ask { askPriceLevel->second.front() };

        // get the min quantity, cant fill a quantity larger then the min
        Quantity quantity { std::min(bid.GetQuantity(), ask.GetQuantity())};

        bid.Fill(quantity);
        ask.Fill(quantity);
        
        // if there is no more quantity remove it from the vector
        if (bid.GetQuantity() == 0) {
          bidPriceLevel->second.erase(bidPriceLevel->second.begin());
        }

        if (ask.GetQuantity() == 0) {
          askPriceLevel->second.erase(askPriceLevel->second.begin());
        }

        // check if the entire price level is empty now
        if (bidPriceLevel->second.empty()) {
          bids_.erase(bidPriceLevel->first);
        }

        if (askPriceLevel->second.empty()) {
          asks_.erase(askPriceLevel->first);
        }
      }
    }
  }


private:
  // TODO: experiment with different data structures and convert Order to
  // pointers
  std::map<Price, Orders, std::greater<Price>> asks_;  // highest ask at top
  std::map<Price, Orders, std::less<Price>> bids_;     // lowest bid at top
                                                       
  // checks if a order was made on a side with a price wether it would match
  // within the current orderbook
  bool CanMatch(Side side, Price price) {
    if (side == Side::Bid) {
      if (asks_.empty()) { return false; }  // cant match stock if there is none
      
      const auto& level { asks_.begin() };
      // return if the price they are bidding is less then the lowest ask
      return price >= level->first;
    } else {
      // is there any bids?
      if (bids_.empty()) { return false;}

      const auto& level { bids_.begin() };
      return price <= level->first;
    }
  }
};

struct TradeInfo
{
  OrderId orderID;
  Side side;
  Price price;
  Quantity quantity;
  TradeType tradeType;
};

