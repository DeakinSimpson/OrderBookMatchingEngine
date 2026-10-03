//
// Created by deakin on 10/3/26.
//

#include "pricelevel.hpp"

OrderEntryPointer PriceLevel::AddOrder(const OrderPointer& orderPointer)
{
    order_pointers_.push_back(orderPointer);

    return {
        std::make_shared<OrderEntry>(orderPointer,std::prev(order_pointers_.end()))
    };
}

OrderPointer PriceLevel::GetFront() const
{
    return order_pointers_.front();
}

void PriceLevel::PopFront()
{
    order_pointers_.erase(order_pointers_.begin());
}

void PriceLevel::Erase(const OrderEntryPointer& orderEntry)
{
    order_pointers_.erase(orderEntry->iterator_);
}
