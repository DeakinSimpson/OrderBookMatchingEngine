#include <thread>
#include "orderbook.hpp"
#include "ingest.hpp"
#include "trade.hpp"
#include "SPSC_Queue.hpp"
#include "Terminal.hpp"

inline static constexpr int D_QUEUE_SIZE { 16777216 };


int main(int argc, char *argv[])
{
    bool isSynthetic { false };

    /* get args */
    std::vector<std::string_view> args(argv + 1, argv + argc);
    std::string filepath;

    if (argc == 1)
    {
        isSynthetic = true;
    }
    else if (argc == 2)
    {
        filepath = std::string(args.at(0));
    } else
    {
        throw(std::invalid_argument("Incorrect Args provided, exiting"));
    }

    /* create atomics for threads */
    std::atomic<bool> ingestComplete { false };
    std::atomic<bool> orderbookComplete { false };

    /* create orderbook and the terminal attached */
    Terminal terminal { D_QUEUE_SIZE, orderbookComplete };
    OrderBook orderBook{ D_QUEUE_SIZE , terminal};

    /* create ingest Queue and its pointers to the ingest methods */
    TSC::Queue<Trade> trades { D_QUEUE_SIZE };
    std::unique_ptr<FileIterator> fileIterator;

    /* create the ingest thread, either real mbo data or synthetic data */
    if (isSynthetic)
    {

    }
    else
    {
        fileIterator = std::make_unique<FileIterator>
            (filepath, trades, ingestComplete);
    }

    /* the main orderbook loop */
    Trade trade;
    for (;;)
    {
        // pop is already a try pop with [[nodiscard]] ommitted
        if (trades.pop(trade))
        {
            trade.MakeTrade(orderBook);
            orderBook.MatchOrders();
            //terminal.AddOrder(trade.GetTradeInfo().price, trade.GetTradeInfo().quantity, trade.GetTradeInfo().side);
            continue;
        }
        if (ingestComplete.load(std::memory_order_acquire))
        {
            while (trades.pop(trade))
            {
                trade.MakeTrade(orderBook);
                orderBook.MatchOrders();
            }
            break;
        }
    }

    /* set the orderbook to cleared for the terminal */
    orderbookComplete.store(true, std::memory_order::release);

    return 0;
}
