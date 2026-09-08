//
// Created by deakin on 9/8/26.
//

#include "orderbook.hpp"

// --- Order ---

void Order::Fill(const Quantity quantity)
{
  if (quantity > quantity_) {
    std::cerr << "Cant Fill Order for More than its Quantity" << std::endl;
  }
  quantity_ -= quantity;
}

// --- OrderBook ---
void OrderBook::AddOrder(const Order& order)
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

void OrderBook::MatchOrders()
{
  // loop through each bid and ask Price
  while (true) {
    // if there are no bids or asks then there is nothing to match
    if (bids_.empty() || asks_.empty())
    { break; }

    const auto& askPriceLevel { asks_.begin() };
    const auto& bidPriceLevel { bids_.begin() };

    // if the best ask is higher then the best bid no orders can match
    if (bidPriceLevel->first < askPriceLevel->first)
    { break; }

    // loop through each bid and ask and try to match at this level
    while (!askPriceLevel->second.empty() && !bidPriceLevel->second.empty()) {
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

Price OrderBook::GetBestBid() const
{
  if (bids_.empty()) { return 0; }
  return bids_.begin()->second.front().GetPrice();
}

Price OrderBook::GetBestAsk() const
{
  if (asks_.empty()) { return 0; }
  return asks_.begin()->second.front().GetPrice();
}

// checks if a order was made on a side with a price wether it would match
// within the current orderbook
bool OrderBook::CanMatch(const Side side, const Price price)
{
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

// --- Trade ---
void Trade::MakeTrade(OrderBook& orderBook) const
{
  if (tradeInfo_.tradeType == TradeType::Add) {
    orderBook.AddOrder({tradeInfo_.orderID, tradeInfo_.side, tradeInfo_.price,
                        tradeInfo_.quantity});
    return;
  }
  // if trade type is none skip
}