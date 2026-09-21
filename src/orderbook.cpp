//
// Created by deakin on 9/8/26.
//

#include "orderbook.hpp"

// --- Order ---

/**
 *
 * @param quantity The Amount that will be removed from the order
 */
void Order::Fill(const Quantity quantity)
{
  if (quantity > quantity_) {
    std::cerr << "Cant Fill Order for More than its Quantity" << std::endl;
  }
  quantity_ -= quantity;
}

// --- OrderBook ---
/**
 *
 * @param order The Order that will be Added to the orderbook
 */
void OrderBook::AddOrder(const OrderPointer& order)
{
  if (orders_.contains(order->GetId())) { return; }

  if (order->GetSide() == Side::Ask) {
    /*
     * 1. Get the price of the order
     * 2. get the orders vector for that price level
     * 3. push back the new order (FIFO)
     * 4. get the iterator of the last element (the one we just inserted)
     * 5. add the order ID and the iterator tot he orders_ hashtable
     */
    auto& orders { asks_[order->GetPrice()] };
    orders.push_back(order);

    const OrderPointers::iterator iterator {
      std::next(orders.begin(), static_cast<ptrdiff_t>(orders.size() - 1))
    };

    orders_.insert({
      order->GetId(),
      std::make_shared<OrderEntry>(OrderEntry{order, iterator})
    });
  } else {
    // same as above for bids_
    auto& orders { bids_[order->GetPrice()] };
    orders.push_back(order);

    OrderPointers::iterator iterator {
      std::next(orders.begin(), static_cast<ptrdiff_t>(orders.size() - 1))
    };

    orders_.insert({
      order->GetId(),
      std::make_shared<OrderEntry>(OrderEntry{order, iterator})
    });
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
        orders_.erase(bid->GetId());
        bids.erase(bids.begin());
      }

      if (ask->GetQuantity() == 0) {
        orders_.erase(ask->GetId());
        asks.erase(asks.begin());
      }
    }

    // check if the entire price level is empty now
    if (bids.empty()) { bids_.erase(bidPrice); }
    if (asks.empty()) { asks_.erase(askPrice); }
  }
}

/**
 *
 * @return The Bid with the highest price
 */
Price OrderBook::GetBestBid() const
{
    if (bids_.empty()) { return 0; }

    return bids_.begin()->second.front()->GetPrice();
}

/**
 *
 * @return The Ask with the lowest price
 */
Price OrderBook::GetBestAsk() const
{
    if (asks_.empty()) { return 0; }

    return asks_.begin()->second.front()->GetPrice();
}

/**
 * @param _side The order side that we are checking for a match
 * @param price The price of the order that we are checking for a match
 * @param side The Side:: that the order is from (opposite to _side)
 *
 * @return True if there is a match, false otherwise
 */
template <typename T>
static bool CheckMatchCondition(const T& _side, const Price price, const Side side)
{
    if (_side.empty())
    {
        return false;
    }

    const auto& [levelPrice, _] = *_side.begin();

    return (side == Side::Ask) ? (price <= levelPrice) : (price >= levelPrice);
}

/**
 *
 * @param side Side of price level check
 * @param price The best price for the given side
 * @return True if there is a match, false otherwise
 */
bool OrderBook::CanMatch(const Side side, const Price price)
{
    if (side == Side::Ask)
    {
        return CheckMatchCondition(bids_, price, side);
    } else
    {
        return CheckMatchCondition(asks_, price, side);
    }
}

/**
 *
 * @param orderID The ID of the order to be canceled
 * @param quantity The Amount of the order to be canceled
 * @return The status of CancelOrder(), Success, OverFill, Fail
 */
CancelStatus OrderBook::CancelOrder(
  const OrderId orderID,
  const Quantity quantity)
{
  // cant cancel order that does not exist
  if (!orders_.contains(orderID)) { return CancelStatus::Fail; }

  const auto entry { orders_.at(orderID) }; // copies pointers
  const auto& order { entry->order_ };
  const auto& iterator { entry->iterator_ };

  // if the cancel amount is less then the total amount we can fill and return
  if (quantity < order->GetQuantity()) {
    order->Fill(quantity);
    return CancelStatus::Success;
  }

  /*
   * Get the return status before removing the item (cant get overfill if gone)
   */
  CancelStatus returnStatus = (quantity == order->GetQuantity())
    ? CancelStatus::Success
    : CancelStatus::OverFill;

  orders_.erase(orderID);

  /*
   * Remove from Sides list, then if the list is empty, remove price level
   */
  Price price { order->GetPrice() };
  if (order->GetSide() == Side::Ask)
  {
    auto& orders { asks_.at(price) };
    orders.erase(iterator);

    if (orders.empty()) { asks_.erase(price); }
  } else
  {
    auto& orders { bids_.at(price) };
    orders.erase(iterator);

    if (orders.empty()) { bids_.erase(price); }
  }

  return returnStatus;
}

/* when a Modify Order comes in, the price and quantity is what the price and
 * quantity is going to be after the modify is complete */
/**
 *
 * @param orderID The ID of the order to be modified
 * @param price The Price that the order will be after modification
 * @param quantity The Quantity that the order will be after modification
 * @return The Status of the order modification, Success or Fail
 */
ModifyStatus OrderBook::ModifyOrder(
  const OrderId orderID,
  const Price price,
  const Quantity quantity)
{
  // cant modify order that does not exist
  if (!orders_.contains(orderID)) { return ModifyStatus::Fail; }

  // get the order and iterator from the orders_ list
  const auto& order { orders_.at(orderID)->order_ };

  // if order is reducing its quantity
  if (order->GetQuantity() > quantity && order->GetPrice() == price) {
    // reduce quantity
    // keep same Price-Time Priority (stays in same spot in queue)
    order->Fill(order->GetQuantity() - quantity);
  } else
  {
    // save temp values that wont persist
    const OrderId t_orderID { order->GetId() };
    const Side t_side { order->GetSide() };

    // cancel the order
    CancelOrder(order->GetId(), order->GetQuantity());

    // add the order to the orderbook
    AddOrder(
      std::make_shared<Order>(Order(t_orderID, t_side, price, quantity))
    );
  }

  return ModifyStatus::Success;
}

// --- Trade ---
/**
 *
 * @param orderBook The orderbook that the trade will be made in
 */
void Trade::MakeTrade(OrderBook& orderBook) const
{
  if (tradeInfo_.tradeType == TradeType::Add){
    orderBook.AddOrder(ToOrderPointer());
  } else if (tradeInfo_.tradeType == TradeType::Cancel) {
    orderBook.CancelOrder(tradeInfo_.orderID, tradeInfo_.quantity);
  } else if (tradeInfo_.tradeType == TradeType::Modify) {
    orderBook.ModifyOrder(
      tradeInfo_.orderID, tradeInfo_.price, tradeInfo_.quantity);
  }
}

/**
 *
 * @return A Pointer to the Order
 */
OrderPointer Trade::ToOrderPointer() const
{
  return std::make_shared<Order>(Order{
    tradeInfo_.orderID,
    tradeInfo_.side,
    tradeInfo_.price,
    tradeInfo_.quantity
  });
}