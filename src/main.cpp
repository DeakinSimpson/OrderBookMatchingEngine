#include <iostream>
#include <map>
#include <vector>
#include <chrono>

enum class OrderType
{
  MarketOrder,
  LimitOrder,
  // TODO: Implement Later
  StopMarketOrder,
  StopLimitOrder,
  TrailingStopOrder,
};

enum class Side
{
  Bid,
  Ask,
};

using OrderId = int;
using Price = int;              
using Quantity = unsigned int;  // cant have negative stock

class Order
{
public:
  Order(OrderId id, Side side, Price price,
    Quantity quantity, OrderType orderType)
      : id_{ id }
      , side_{ side }
      , price_{ price }
      , quantity_{ quantity }
      , orderType_{ orderType }
  {  };

  OrderId GetId() { return id_; }
  Side GetSide() { return side_; }
  Price GetPrice() { return price_; }
  Quantity GetQuantity() { return quantity_; }
  OrderType GetOrderType() { return orderType_; }
  
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
  OrderType orderType_;
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

int main () {
  OrderBook orderBook {};

  Orders orders;
  orders.reserve(100);

  for (OrderId id = 0; id < 100; ++id) {
    Side side = (id % 2 == 0) ? Side::Ask : Side::Bid;
    Price price = 10 + (id % 5);           // spreads prices 10-14
    Quantity quantity = static_cast<Quantity>((id % 10) + 1); // quantities 1-10
    orders.emplace_back(id, side, price, quantity, OrderType::LimitOrder);
  }
  
  auto addStart { std::chrono::high_resolution_clock::now() };
  for (auto& order : orders) {
    orderBook.AddOrder(order);
  }
  auto addEnd { std::chrono::high_resolution_clock::now() };
  auto addDuration = std::chrono::duration_cast<std::chrono::nanoseconds>
    (addEnd - addStart);

  auto matchStart { std::chrono::high_resolution_clock::now() };
  orderBook.MatchOrders();
  auto matchEnd { std::chrono::high_resolution_clock::now() };
  auto matchDuration { std::chrono::duration_cast<std::chrono::nanoseconds>
    (matchEnd - matchStart)};
  
  std::cout << "add duration: " << addDuration.count() << std::endl;
  std::cout << "match duration: " << matchDuration.count() << std::endl;
  return 0;
}
