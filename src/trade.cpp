//
// Created by deakin on 10/3/26.
//

#include "trade.hpp"

/**
 *
 * @param orderBook The orderbook that the trade will be made in
 */
void Trade::MakeTrade(OrderBook &orderBook) const
{
    switch (tradeInfo_.tradeType)
    {
        case TradeType::Add:
            orderBook.AddOrder(ToOrderPointer());
            break;
        case TradeType::Cancel:
            orderBook.CancelOrder(tradeInfo_.orderID, tradeInfo_.quantity);
            break;
        case TradeType::Modify:
            orderBook.ModifyOrder(
                tradeInfo_.orderID, tradeInfo_.price, tradeInfo_.quantity);
            break;
        default: break;
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