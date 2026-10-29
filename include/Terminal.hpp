//
// Created by deakin on 10/29/26.
//

#pragma once
#include <iostream>
#include <map>
#include <atomic>
#include <stdexcept>
#include <stop_token>
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
        , thread_ { [this](std::stop_token st) { Thread(st); } }
    {  };

    void Thread(std::stop_token st)
    {
        try
        {
            while (!st.stop_requested())
            {
                if (ProcessOrder())
                {
                    if (ordersProcessed == 0) { PrintToTerminal(); }

                    ++ordersProcessed;
                    ordersProcessed %= ORDERS_PROCESSED_BEFORE_TERMINAL_DISPLAY;
                }
                else if (ingestComplete_.load(std::memory_order_acquire))
                {
                    break;
                }
                else
                {
                    std::this_thread::yield();
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

    void AddOrder(Price price, Quantity quantity, Side side, bool isAddOrder = true)
    {
        queue_.push(TerminalPacket{price, quantity, side, isAddOrder});
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
        std::cout << "___BIDS___" << std::endl;
        for (auto [price, quantity] : bids_)
        {
            std::cout << "$" << price << ": " << quantity << std::endl;
        }


        std::cout << "___ASKS___" << std::endl;
        for (auto [price, quantity] : asks_)
        {
            std::cout << "$" << price << ": " << quantity << std::endl;
        }
    }

private:
    std::map<Price, Quantity> bids_;
    std::map<Price, Quantity> asks_;
    TSC::Queue<TerminalPacket> queue_;
    std::atomic<bool>& ingestComplete_;
    int ordersProcessed {};
    std::jthread thread_;
};
