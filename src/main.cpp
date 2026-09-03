#include <iostream>
#include <map>

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
  void Fil(Quantity quantity) { quantity_ -= quantity; }

private:
  OrderId id_;
  Side side_;
  Price price_;
  Quantity quantity_;
  OrderType orderType_;
};

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
      asks_.insert(std::pair(order.GetPrice(), order));
    } else {
      bids_.insert(std::pair(order.GetPrice(), order));
    }
  }

private:
  std::map<Price, Order> asks_;
  std::map<Price, Order> bids_;
};

int main () {
  OrderBook orderBook {};
  Order order { 0, Side::Ask, 10, 5, OrderType::LimitOrder };
  orderBook.AddOrder(order);

  return 0;
}
