#include <thread>
#include "orderbook.hpp"
#include "ingest.hpp"
#include "trade.hpp"
#include "SPSC_Queue.hpp"

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
    OrderBook orderBook{1000000};
    TSC::Queue<Trade> trades { 1000000 };


    std::jthread ingestThread([&]() {
        IngestThread(trades, std::string(args.at(0)), ingestComplete);
    });

    Trade trade;
    while (!(trades.empty() && ingestComplete))
    {
        trades.pop(trade);
        trade.MakeTrade(orderBook);
        orderBook.MatchOrders();
    }

    return 0;
}
