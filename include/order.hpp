//
// Created by deakin on 10/3/26.
//

#pragma once

#include <cstdint>

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