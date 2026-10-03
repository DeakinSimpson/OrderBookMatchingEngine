//
// Created by deakin on 10/3/26.
//

#include "order.hpp"

#include <iostream>


/**
 *
 * @param quantity The Amount that will be removed from the order
 */
void Order::Fill(const Quantity quantity)
{
    if (quantity > quantity_)
    {
        std::cerr << "Cant Fill Order for More than its Quantity" << std::endl;
        return;
    }
    quantity_ -= quantity;
}