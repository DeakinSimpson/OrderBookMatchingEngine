//
// Created by deakin on 10/29/26.
//

#pragma once
#include <iostream>
#include <map>
#include <atomic>
#include <thread>
#include <vector>

#include "order.hpp"
#include "SPSC_Queue.hpp"

/* Default values */
namespace TerminalDefault
{
    const std::vector<Price> DEFAULT_PRICE_LEVELS
    {
        0,
        100000000000,
        150000000000,
        200000000000,
        210000000000,
        215000000000,
        216000000000,
        217000000000,
        218000000000,
        219000000000,
        220000000000,
        230000000000,
        240000000000,
        250000000000,
        300000000000,
        350000000000,
        400000000000,
        450000000000,
        500000000000
    };

    constexpr int DEFAULT_BAR_WIDTH { 100 };
    constexpr int ordersProcessed { 1 };
    constexpr int priceCharWidth { 3 };
    constexpr int quantityCharWidth { 10 };
    constexpr int refreshRate { 1000 / 60 };
}

class Terminal
{
    struct TerminalPacket
    {
        Price price;
        Quantity quantity;
        Side side;
        bool isAddOrder;
    };
public:
    Terminal(const std::size_t queueSize, std::atomic<bool>& orderbookComplete,
        const std::vector<Price>& buckets = TerminalDefault::DEFAULT_PRICE_LEVELS)
        : bids_ { }, asks_ { }, buckets_ { buckets }, queue_{queueSize}
        , orderbookComplete_{orderbookComplete}
        , thread_ { [this]() { Thread(); } }
    {  };



    void AddOrder(Price price, Quantity quantity, Side side, bool isAddOrder = true)
    {
        queue_.push(TerminalPacket{price, quantity, side, isAddOrder});
    }

    void IncrementTradesMatched() { ++tradesMatched_; }

private:
    std::map<Price, Quantity>   bids_;
    std::map<Price, Quantity>   asks_;
    std::vector<Price>          buckets_;
    TSC::Queue<TerminalPacket>  queue_;
    std::atomic<bool>&          orderbookComplete_;
    int                         ordersProcessed {};
    std::jthread                thread_;
    Quantity                    maxBidQuantity_ { 0 };
    Quantity                    maxAskQuantity_ { 0 };
    uint64_t                    tradesMatched_ { 0 };

    void Thread(const int barWidth = TerminalDefault::DEFAULT_BAR_WIDTH);
    bool ProcessOrder();
    void PrintToTerminal(const int barWidth);
    void AddOrderToSide(TerminalPacket& terminalPacket, auto& side);
    void DrawBar(const float progress, const int charWidth, const bool isBid);
    Price FindBucket(const Price price);

    double Price_lltod(uint64_t price)
    {
        return static_cast<double>(price) / 1e9;
    }

    // poor naming might change latter
    Price Price_dtoll(double price)
    {
        return static_cast<Price>(price * 1e9);
    }

    void ClearScreen()
    {
        std::cout << "\033[2J\033[H" << std::flush;
    }
};
