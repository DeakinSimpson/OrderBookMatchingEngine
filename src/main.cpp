#include <thread>
#include "orderbook.hpp"
#include "ingest.hpp"
#include "trade.hpp"
#include "SPSC_Queue.hpp"
#include "Terminal.hpp"

int main(int argc, char *argv[])
{
    std::vector<std::string_view> args(argv + 1, argv + argc);
    std::atomic<bool> ingestComplete { false };
    std::atomic<bool> orderbookComplete { false };
    Terminal terminal { 1048576, orderbookComplete };
    OrderBook orderBook{ 1048576 , terminal};
    TSC::Queue<Trade> trades { 1048576 };
    const std::string filepath { std::string(args.at(0)) };
    FileIterator fileIterator { filepath, trades, ingestComplete };


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

    orderbookComplete.store(true, std::memory_order::release);

    return 0;
}
