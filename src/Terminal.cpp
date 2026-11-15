// //
// // Created by deakin on 10/29/26.
// //

#include "Terminal.hpp"

void Terminal::Thread()
{
    using clock = std::chrono::steady_clock;
    constexpr auto refresh = std::chrono::milliseconds(100);   // 10 frames/sec
    auto nextPrint = clock::now();

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

bool Terminal::ProcessOrder()
{
    TerminalPacket terminalPacket;
    if (!queue_.pop(terminalPacket))
    {
        return false;
    }

    auto& side { (terminalPacket.side == Side::Ask) ? asks_ : bids_ };

    if (terminalPacket.isAddOrder)
    {
        AddOrderToSide(terminalPacket, side);
    }
    else
    {
        if (side[terminalPacket.price] < terminalPacket.quantity)
        {
            throw std::invalid_argument("CANT remove more items then exist");
        }

        AddOrderToSide(terminalPacket, side);
    }

    return true;
}

void Terminal::AddOrderToSide(TerminalPacket &terminalPacket, auto &side)
{
    Price bucket {};
    for (const auto& priceLevel : buckets_)
    {
        if (terminalPacket.price > priceLevel)
        {
            bucket = priceLevel;
        }
        else
        {
            break;
        }
    }
    if (terminalPacket.isAddOrder)
    {
        side[bucket] += terminalPacket.quantity;
    }
    else
    {
        side[bucket] -= terminalPacket.quantity;
    }
}

void Terminal::PrintToTerminal()
{
    ClearScreen();
    const auto numBuckets { buckets_.size() };

    std::cout << "___ASKS___" << std::endl;
    for (auto i { numBuckets }; i-- > 0; )
    {
        if (asks_[buckets_[i]] > 0)
        {
            std::cout << "$" << Price_lltod(buckets_[i])  << ": " << asks_[buckets_[i]] << std::endl;
        }
    }

    std::cout << "___BIDS___" << std::endl;
    for (auto i { numBuckets}; i-- > 0; )
    {
        if (bids_[buckets_[i]] > 0)
        {
            std::cout << "$" << Price_lltod(buckets_[i])  << ": " << bids_[buckets_[i]] << std::endl;
        }
    }
}
