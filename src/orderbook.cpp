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
void OrderBook::AddOrder(const OrderPointer& order)
{
  if (order->GetSide() == Side::Ask) {
    /*
     * 1. Get the price of the order
     * 2. get the orders vector for that price level
     * 3. push back the new order (FIFO)
     * 4. get the iterator of the last element (the one we just inserted)
     * 5. add the order ID and the iterator tot he orders_ hashtable
     */
    const Price price { order->GetPrice() };
    auto& orders { asks_[price] };
    orders.push_back(order);

    OrderPointers::iterator iterator {
      std::next(orders.begin(), orders.size() - 1)
    };
    orders_.insert({order->GetId(), {order, iterator}});
  } else {
    // same as above for bids_
    const Price price { order->GetPrice() };
    auto& orders { bids_[price] };
    orders.push_back(order);

    OrderPointers::iterator iterator {
      std::next(orders.begin(), orders.size() - 1)
    };
  }
}

void OrderBook::MatchOrders()
{
  // loop through each bid and ask Price
  while (true) {
    // if there are no bids or asks then there is nothing to match
    if (bids_.empty() || asks_.empty())
    { break; }

    auto& [askPrice, asks] { *asks_.begin() };
    auto& [bidPrice, bids] { *bids_.begin() };

    // if the best ask is higher then the best bid no orders can match
    if (bidPrice < askPrice) { break; }

    // loop through each bid and ask and try to match at this level
    while (!asks.empty() && !bids.empty()) {
      // FIFO, get fist value of vector
      auto& bid { bids.front() };
      auto& ask { asks.front() };

      // get the min quantity, cant fill a quantity larger then the min
      Quantity quantity { std::min(bid->GetQuantity(), ask->GetQuantity())};

      bid->Fill(quantity);
      ask->Fill(quantity);

      // if there is no more quantity remove it from the vector
      if (bid->GetQuantity() == 0) {
        bids.erase(bids.begin());
      }

      if (ask->GetQuantity() == 0) {
        asks.erase(asks.begin());
      }

      // check if the entire price level is empty now
      if (bids.empty()) {
        bids_.erase(bidPrice);
      }

      if (asks.empty()) {
        asks_.erase(askPrice);
      }
    }
  }
}

Price OrderBook::GetBestBid() const
{
  if (bids_.empty()) { return 0; }
  return bids_.begin()->second.front()->GetPrice();
}

Price OrderBook::GetBestAsk() const
{
  if (asks_.empty()) { return 0; }
  return asks_.begin()->second.front()->GetPrice();
}

// checks if a order was made on a side with a price wether it would match
// within the current orderbook
bool OrderBook::CanMatch(const Side side, const Price price)
{
  if (side == Side::Bid) {
    if (asks_.empty()) { return false; }  // cant match stock if there is none

    const auto& [levelPrice, _] { *asks_.begin() };
    // return if the price they are bidding is less then the lowest ask
    return price >= levelPrice;
  } else {
    // is there any bids?
    if (bids_.empty()) { return false;}

    const auto& [levelPrice, _] { *bids_.begin() };
    return price <= levelPrice;
  }
}

// --- Trade ---
void Trade::MakeTrade(OrderBook& orderBook) const
{
  if (tradeInfo_.tradeType == TradeType::Add) {
    orderBook.AddOrder(ToOrderPointer());
    return;
  }
  // if trade type is none skip
}

OrderPointer Trade::ToOrderPointer() const
{
  return std::make_shared<Order>(Order{
    tradeInfo_.orderID,
    tradeInfo_.side,
    tradeInfo_.price,
    tradeInfo_.quantity
  });
}