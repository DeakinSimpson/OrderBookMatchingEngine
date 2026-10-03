#include "orderbook.hpp"
#include "ingest.hpp"
#include "trade.hpp"

int main(int argc, char *argv[])
{
    std::vector<std::string_view> args(argv + 1, argv + argc);
    FileIterator fi{args.at(0).data()};

    OrderBook orderBook{1000000};

    MboMessage msg;

    while (fi.Next(msg))
    {
        Trade trade{fi.GetTradeInfo(msg)};
        trade.MakeTrade(orderBook);
        orderBook.MatchOrders();
    }

    return 0;
}
