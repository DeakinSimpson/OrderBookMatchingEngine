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

  orderBook.AddOrder(std::make_shared<Order>(order1));
  orderBook.AddOrder(std::make_shared<Order>(order2));
  orderBook.AddOrder(std::make_shared<Order>(order3));

  orderBook.MatchOrders();

  EXPECT_EQ(orderBook.GetBestAsk(), 101.0);
}

TEST(OrderBookTest, DontMatchIfBestAskNotMet) {
  OrderBook orderBook {};
  Order order1 { 0, Side::Ask, 100.0, 10 };
  Order order2 { 0, Side::Bid, 99.0, 10 };

  orderBook.AddOrder(std::make_shared<Order>(order1));
  orderBook.AddOrder(std::make_shared<Order>(order2));

  orderBook.MatchOrders();

  EXPECT_EQ(orderBook.GetBestAsk(), 100.0);
}

TEST(OrderBookTest, AskWithHigherQuantityKept) {
  OrderBook orderBook {};
  Order order1 { 0, Side::Ask, 100.0, 10 };
  Order order2 { 1, Side::Bid, 100.0, 15 };

  orderBook.AddOrder(std::make_shared<Order>(order1));
  orderBook.AddOrder(std::make_shared<Order>(order2));

  orderBook.MatchOrders();

  EXPECT_EQ(orderBook.GetBestBid(), 100.0);
}

TEST(OrderBookTest, BidWithHigherQuantityKept) {  OrderBook orderBook {};
  Order order1 { 0, Side::Ask, 100.0, 15 };
  Order order2 { 1, Side::Bid, 100.0, 10 };

  orderBook.AddOrder(std::make_shared<Order>(order1));
  orderBook.AddOrder(std::make_shared<Order>(order2));

  orderBook.MatchOrders();

  EXPECT_EQ(orderBook.GetBestAsk(), 100.0);

}

TEST(OrderBookTest, AskDeletedWhenFilled) {
  OrderBook orderBook {};
  Order order1 { 0, Side::Ask, 100.0, 10 };
  Order order2 { 1, Side::Bid, 100.0, 15 };

  orderBook.AddOrder(std::make_shared<Order>(order1));
  orderBook.AddOrder(std::make_shared<Order>(order2));

  orderBook.MatchOrders();

  EXPECT_EQ(orderBook.GetBestAsk(), 0.0);

}

TEST(OrderBookTest, BidDeletedWhenFilled) {
  OrderBook orderBook {};
  Order order1 { 0, Side::Ask, 100.0, 15 };
  Order order2 { 1, Side::Bid, 100.0, 10 };

  orderBook.AddOrder(std::make_shared<Order>(order1));
  orderBook.AddOrder(std::make_shared<Order>(order2));

  orderBook.MatchOrders();

  EXPECT_EQ(orderBook.GetBestBid(), 0.0);
}

TEST(OrderBookTest, GetBestBidOnEmptyOrderBook) {
  OrderBook orderBook {};

  EXPECT_EQ(orderBook.GetBestBid(), 0.0);
}

TEST(OrderBookTest, GetBestAskOnEmptyOrderBook) {
  OrderBook orderBook {};

  EXPECT_EQ(orderBook.GetBestAsk(), 0.0);
}

TEST(OrderBookTest, CancelOrderAfterFullyCanceled) {
  OrderBook orderBook {};

  Order order1 {0, Side::Ask, 10, 10 };

  orderBook.AddOrder(std::make_shared<Order>(order1));

  const CancelStatus status1 { orderBook.CancelOrder(0, 10) };
  const CancelStatus status2 { orderBook.CancelOrder(0, 10) };

  EXPECT_EQ(status1, CancelStatus::Success);
  EXPECT_EQ(status2, CancelStatus::Fail);
}

TEST(OrderBookTest, CancelOrderThatNeverExisted) {
  OrderBook orderbook;

  const CancelStatus status { orderbook.CancelOrder(0, 10) };

  EXPECT_EQ(status, CancelStatus::Fail);
}