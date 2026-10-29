// //
// // Created by deakin on 10/29/26.
// //
//
// #include "Terminal.hpp"
//
// void Terminal::AddOrder(const Price price, const Quantity quantity,
//                         const Side side, const bool isAddOrder)
// {
//     auto& orderSide = (side == Side::Ask) ? asks_ : bids_;
//
//     if (isAddOrder)
//     {
//         orderSide.at(price) += quantity;
//     }
//     else
//     {
//         if (quantity > orderSide.at(price))
//         {
//             throw std::invalid_argument
//                 ("CANNOT remove quantity larger then existing quantity");
//         }
//         orderSide.at(price) -= quantity;
//     }
// }
