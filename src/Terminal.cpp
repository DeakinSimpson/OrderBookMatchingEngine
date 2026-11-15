// //
// // Created by deakin on 10/29/26.
// //

#include "Terminal.hpp"

#include <iomanip>

void Terminal::Thread(const int barWidth)
{
    using clock = std::chrono::steady_clock;
    constexpr auto refresh = std::chrono::milliseconds(100);   // 10 frames/sec
    auto nextPrint = clock::now();
    for (;;)
    {
        if (ProcessOrder())
        {
            if (++ordersProcessed % TerminalDefault::ordersProcessed == 0
                && clock::now() >= nextPrint)
            {
                PrintToTerminal(barWidth);
                nextPrint = clock::now() + refresh;
            }
        }
        else if (ingestComplete_.load(std::memory_order_acquire))
        {
            break;
        }
    }
    while (ProcessOrder()) {}
    PrintToTerminal(barWidth);
}

bool Terminal::ProcessOrder()
{
    /* Get the terminalPacket from the terminalPacket queue */
    TerminalPacket terminalPacket;
    if (!queue_.pop(terminalPacket))
    {
        return false;
    }

    auto& side { (terminalPacket.side == Side::Ask) ? asks_ : bids_ };

    /* Add order to the corresponding bucket */
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
    /* Get the bucket that the terminalPacket corresponds too */
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

    /* Add the quantity to the bucket */
    if (terminalPacket.isAddOrder)
    {
        side[bucket] += terminalPacket.quantity;
    }
    else
    {
        side[bucket] -= terminalPacket.quantity;
    }

    /* Check if the bucket is now larger then the current max */
    if (side[bucket] > maxBidQuantity_ && terminalPacket.side == Side::Bid)
    {
        maxBidQuantity_ = side[bucket];
    }
    else if (side[bucket] > maxAskQuantity_ && terminalPacket.side == Side::Ask)
    {
        maxAskQuantity_ = side[bucket];
    }
}

void Terminal::PrintToTerminal(const int barWidth)
{
    ClearScreen();
    const auto numBuckets { buckets_.size() };

    std::cout << std::setw(TerminalDefault::priceCharWidth
        + TerminalDefault::quantityCharWidth
        + TerminalDefault::DEFAULT_BAR_WIDTH + 3) << "";
    std::cout << "______ASKS______" << std::endl;
    for (auto i { numBuckets }; i-- > 0; )
    {
        if (asks_[buckets_[i]] > 0)
        {
            std::cout << std::setw(TerminalDefault::priceCharWidth
                + TerminalDefault::quantityCharWidth + 3) << "";
            DrawBar(static_cast<float>(asks_[buckets_[i]]) /
                    static_cast<float>(maxAskQuantity_), barWidth, false);
            std::cout << "$" << std::right
                << std::setw(TerminalDefault::priceCharWidth)
                << Price_lltod(buckets_[i])  << ": ";
            std::cout << std::left
                << std::setw(TerminalDefault::quantityCharWidth)
                << asks_[buckets_[i]];
            std::cout << std::endl;
        }
    }

    std::cout << "______BIDS______" << std::endl;
    for (auto i { numBuckets}; i-- > 0; )
    {
        if (bids_[buckets_[i]] > 0)
        {
            std::cout << "$" << std::right
                << std::setw(TerminalDefault::priceCharWidth)
                << Price_lltod(buckets_[i])  << ": ";
            std::cout << std::left
                << std::setw(TerminalDefault::quantityCharWidth)
                << bids_[buckets_[i]];
            DrawBar(static_cast<float>(bids_[buckets_[i]]) /
                static_cast<float>(maxBidQuantity_), barWidth, true);
            std::cout << std::endl;
        }
    }
}

void Terminal::DrawBar(const float progress, const int charWidth, const bool isBid)
{
    const std::string color { (isBid) ? "\033[32m" : "\033[91m" };

    const int edge { static_cast<int>(progress * (float)charWidth) };

    /* Print left to right if bis */
    if (isBid)
    {
        for (int i {}; i < charWidth; ++i)
        {
            if (i < edge)
            {
                std::cout << color << "█";
            }
            else
            {
                std::cout << color << "░";
            }
        }
    }
    else /* Print in reverse and red if ask */
    {
        for (int i {}; i < charWidth; ++i)
        {
            if (i < charWidth - edge)
            {
                std::cout << color << "░";
            }
            else
            {
                std::cout << color << "█";
            }
        }
    }

    std::cout << "\033[0m";
}
