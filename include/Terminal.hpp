//
// Created by deakin on 10/29/26.
//

#pragma once
#include <iostream>
#include <map>
#include <atomic>
#include <stdexcept>
#include <thread>
#include <vector>

#include "order.hpp"
#include "SPSC_Queue.hpp"

constexpr int ORDERS_PROCESSED_BEFORE_TERMINAL_DISPLAY { 1000 };

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
    Terminal(const std::size_t queueSize, std::atomic<bool>& ingestComplete,
        const std::vector<Price>& buckets =
            { 0, 100000000000, 150000000000, 200000000000,
              250000000000, 300000000000, 350000000000,
              400000000000, 450000000000, 500000000000 })
        : bids_ { }, asks_ { }, buckets_ { buckets }, queue_{queueSize}
        , ingestComplete_{ingestComplete}
        , thread_ { [this]() { Thread(); } }
    {  };



    void AddOrder(Price price, Quantity quantity, Side side, bool isAddOrder = true)
    {
        queue_.push(TerminalPacket{price, quantity, side, isAddOrder});
    }

private:
    std::map<Price, Quantity> bids_;
    std::map<Price, Quantity> asks_;
    std::vector<Price> buckets_;
    TSC::Queue<TerminalPacket> queue_;
    std::atomic<bool>& ingestComplete_;
    int ordersProcessed {};
    std::jthread thread_;

    void Thread();
    bool ProcessOrder();
    void PrintToTerminal();
    void AddOrderToSide(TerminalPacket& terminalPacket, auto& side);

    double Price_lltod(uint64_t price)
    {
        return static_cast<double>(price) / 1e9;
    }

    // poor naming might change latter
    Price Price_dtoll(double price)
    {
        return static_cast<Price>(price * 1e9);
    }

    void PrintBuckets(auto& side)
    {

    }

    void ClearScreen()
    {
        std::cout << "\033[2J\033[H" << std::flush;
    }
};
