//
// Created by deakin on 9/6/26.
//
#include <gtest/gtest.h>
#include "orderbook.hpp"

TEST(OrderBookTest, PriceTimePriority) {
  OrderBook orderBook {};
  Order order1 { 0, Side::Ask, 101.0, 10 };
  Order order2 { 1, Side::Ask, 100.0, 10 };
  Order order3 { 2, Side::Bid, 101.0, 10 };
  Order order4 { 3, Side::Bid, 99, 10};

  orderBook.AddOrder(order1);
  orderBook.AddOrder(order2);
  orderBook.AddOrder(order3);

  orderBook.MatchOrders();

  std::cout << "Best Ask TEST: " << orderBook.GetBestAsk() << std::endl;

  EXPECT_EQ(orderBook.GetBestAsk(), 101.0);
}
