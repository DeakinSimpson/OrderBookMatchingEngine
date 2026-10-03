//
// Created by deakin on 10/3/26.
//

#pragma once

#include "order.hpp"

class PriceLevel
{
    Price price_ {};
    OrderPointers order_pointers_;

public:

    PriceLevel() = default;
    PriceLevel(Price price) : price_ { price } {  }

    bool operator==(const PriceLevel &) const = default;
    auto operator<=>(const PriceLevel &) const = default;

    [[nodiscard]] Price GetPrice() const { return price_; }
    [[nodiscard]] OrderPointer GetFront() const;
    [[nodiscard]] bool IsEmpty() const { return order_pointers_.empty(); }

    void PopFront();
    void Erase(const OrderEntryPointer& it);

    OrderEntryPointer AddOrder(const OrderPointer& orderPointer);
};
