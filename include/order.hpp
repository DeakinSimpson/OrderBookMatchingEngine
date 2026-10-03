//
// Created by deakin on 10/3/26.
//

#pragma once

#include <cstdint>
#include <list>
#include <memory>

enum class Side
{
    Bid,
    Ask,
    None,
};

enum class TradeType
{
    Add,
    Cancel,
    Modify,
    None,
};

using OrderId = uint64_t;
using Price = uint64_t;
using Quantity = uint32_t; // cant have negative stock

class Order; // forward declare

/*
 * Original thought was to use std::vector as it has better cache locality,
 * and O(1) random access whereas list has O(n), however because vector
 * invalidates iterators where there is a insertion or deletion i have chosen
 * to use list instead
 */
using OrderPointer = std::shared_ptr<Order>;
using OrderPointers = std::list<OrderPointer>;

struct OrderEntry
{
    OrderPointer order_{};
    OrderPointers::iterator iterator_;
};

class Order
{
public:
    Order(const OrderId id, const Side side, Price const price,
          const Quantity quantity)
        : id_{id}
    , side_{side}
    , price_{price}
    , quantity_{quantity}
    {  };

    // [[nodiscard]] means that we cant call this as a no return function
    [[nodiscard]] OrderId GetId() const { return id_; }
    [[nodiscard]] Side GetSide() const { return side_; }
    [[nodiscard]] Price GetPrice() const { return price_; }
    [[nodiscard]] Quantity GetQuantity() const { return quantity_; }

    void Fill(Quantity quantity);

private:
    OrderId id_;
    Side side_;
    Price price_;
    Quantity quantity_;
};