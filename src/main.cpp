#include <cfloat>
#include <iostream>
#include <sys/types.h>

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

private:
  OrderId id_;
  Side side_;
  Price price_;
  Quantity quantity_;
  OrderType orderType_;
};

int main () {
  std::cout << "Hello Test!" << std::endl;

  return 0;
}
