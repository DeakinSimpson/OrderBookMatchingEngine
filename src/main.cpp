#include <iostream>
#include <map>
#include <vector>

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
  Order order { 0, Side::Ask, 10, 5, OrderType::LimitOrder };
  orderBook.AddOrder(order);

  return 0;
}
