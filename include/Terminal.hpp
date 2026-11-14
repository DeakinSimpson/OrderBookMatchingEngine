//
// Created by deakin on 10/29/26.
//

#pragma once
#include <iostream>
#include <map>
#include <atomic>
#include <stdexcept>
#include <thread>

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
    Terminal(const std::size_t queueSize, std::atomic<bool>& ingestComplete)
        : bids_ { }, asks_ { }, queue_{queueSize}
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
    TSC::Queue<TerminalPacket> queue_;
    std::atomic<bool>& ingestComplete_;
    int ordersProcessed {};
    std::jthread thread_;

    void Thread()
    {
        using clock = std::chrono::steady_clock;
        constexpr auto refresh = std::chrono::milliseconds(100);   // 10 frames/sec
        auto nextPrint = clock::now();

        try
        {
            for (;;)
            {
                if (ProcessOrder())
                {
                    if (++ordersProcessed % 1024 == 0 && clock::now() >= nextPrint)
                    {
                        PrintToTerminal();
                        nextPrint = clock::now() + refresh;
                    }
                }
                else if (ingestComplete_.load(std::memory_order_acquire))
                {
                    break;
                }
            }
            while (ProcessOrder()) {}
            PrintToTerminal();
        }
        catch (const std::exception& e)
        {
            std::cerr << "Terminal thread error: " << e.what() << '\n';
        }
    }

    bool ProcessOrder()
    {
        TerminalPacket terminalPacket;
        if (!queue_.pop(terminalPacket))
        {
            return false;
        }

        auto& side { (terminalPacket.side == Side::Ask) ? asks_ : bids_ };

        if (terminalPacket.isAddOrder)
        {
            side[terminalPacket.price] += terminalPacket.quantity;
        }
        else
        {
            if (side[terminalPacket.price] < terminalPacket.quantity)
            {
                throw std::invalid_argument("CANT remove more items then exist");
            }
            side[terminalPacket.price] -= terminalPacket.quantity;
        }

        return true;
    }

    void PrintToTerminal()
    {
        ClearScreen();

        std::cout << "___BIDS___" << std::endl;
        for (auto [price, quantity] : bids_)
        {
            std::cout << "$" << Price_lltod(price) << ": " << quantity << std::endl;
        }


        std::cout << "___ASKS___" << std::endl;
        for (auto [price, quantity] : asks_)
        {
            std::cout << "$" << Price_lltod(price)  << ": " << quantity << std::endl;
        }
    }

    double Price_lltod(uint64_t price)
    {
        return static_cast<double>(price) / 1e9;
    }

    // void PrintBuckets(auto& side, std::vector<Price> buckets = { })
    // {
    //
    // }

    void ClearScreen()
    {
        std::cout << "\033[2J\033[H" << std::flush;
    }
};
