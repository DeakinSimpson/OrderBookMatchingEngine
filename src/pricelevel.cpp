//
// Created by deakin on 10/3/26.
//

#include "pricelevel.hpp"

OrderPointers::iterator PriceLevel::AddOrder(const OrderPointer& orderPointer)
{
    order_pointers_.push_back(orderPointer);

    return std::prev(order_pointers_.end());
}

void PriceLevel::CancelOrder(const OrderPointers::iterator& it)
{
    order_pointers_.erase(it);
}

OrderPointer PriceLevel::GetFront() const
{
    return order_pointers_.front();
}

void PriceLevel::PopFront()
{
    order_pointers_.erase(order_pointers_.begin());
}

void PriceLevel::Erase(const OrderPointers::iterator &it)
{
    order_pointers_.erase(it);
}
