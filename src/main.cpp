#include <chrono>
#include <iostream>
#include "orderbook.hpp"

int main () {
  OrderBook orderBook {};

  Orders orders;
  orders.reserve(100);

  for (OrderId id = 0; id < 100; ++id) {
    Side side = (id % 2 == 0) ? Side::Ask : Side::Bid;
    Price price = (Price) 10 + static_cast<Price>(id % 5);           // spreads prices 10-14
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
