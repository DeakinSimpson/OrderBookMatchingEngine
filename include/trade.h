//
// Created by deakin on 10/3/26.
//

#pragma once

#include "order.hpp"
#include "orderbook.hpp"

struct TradeInfo
{
    OrderId orderID;
    Side side;
    Price price;
    Quantity quantity;
    TradeType tradeType;
};

class Trade
{
    TradeInfo tradeInfo_;

public:
    explicit Trade(const TradeInfo &tradeInfo) : tradeInfo_{tradeInfo}
    {
    }

    [[nodiscard]] TradeInfo GetTradeInfo() const { return tradeInfo_; }

    [[nodiscard]] OrderPointer ToOrderPointer() const;

    void MakeTrade(OrderBook &orderBook) const;
};