#include <thread>
#include "orderbook.hpp"
#include "ingest.hpp"
#include "trade.hpp"
#include "SPSC_Queue.hpp"
#include "Terminal.hpp"

// the code that will be run on the ingest thread
void IngestThread(TSC::Queue<Trade> &trades, const std::string& filepath, std::atomic<bool>& isComplete)
{
    FileIterator fileIterator { filepath };
    MboMessage msg;
    while (fileIterator.Next(msg))
    {
        trades.push(fileIterator.GetTradeInfo(msg));
    }

    isComplete.store(true);
}

int main(int argc, char *argv[])
{
    std::vector<std::string_view> args(argv + 1, argv + argc);
    std::atomic<bool> ingestComplete { false };
    Terminal terminal { 1048576, ingestComplete };
    OrderBook orderBook{ 1048576 , terminal};
    TSC::Queue<Trade> trades { 1048576 };


    std::jthread ingestThread([&]() {
        IngestThread(trades, std::string(args.at(0)), ingestComplete);
    });

    Trade trade;
    for (;;)
    {
        // pop is already a try pop with [[nodiscard]] ommitted
        if (trades.pop(trade))
        {
            trade.MakeTrade(orderBook);
            orderBook.MatchOrders();
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

    return 0;
}
