#include "orderbook.hpp"
#include "ingest.hpp"
int main (int argc, char* argv[]) {
  std::vector<std::string_view> args(argv + 1, argv + argc);
  FileIterator fi { args.at(0).data() };

  OrderBook orderBook {};

  while (!fi.IsEOF()) {
    Trade trade {fi.GetTradeInfo()};
    trade.MakeTrade(orderBook);
    orderBook.MatchOrders();
  }

  return 0;
}
