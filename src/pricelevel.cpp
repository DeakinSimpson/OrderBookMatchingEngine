//
// Created by deakin on 10/3/26.
//

#include "pricelevel.hpp"

OrderEntryPointer PriceLevel::AddOrder(const OrderPointer& orderPointer)
{
    order_pointers_.push_back(orderPointer);

    return
        std::make_shared<OrderEntry>(orderPointer,std::prev(order_pointers_.end()));
}

OrderPointer PriceLevel::GetFront() const
{
    return order_pointers_.front();
}

void PriceLevel::PopFront()
{
    const auto it { order_pointers_.begin() };
    total_quantity_ -= (**it).GetQuantity();
    order_pointers_.erase(it);
}

void PriceLevel::Erase(const OrderEntryPointer& orderEntry)
{
    order_pointers_.erase(orderEntry->iterator_);
}
