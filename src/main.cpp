#include <chrono>
#include <iostream>
#include "orderbook.hpp"
#include "ingest.hpp"
int main () {
  FileIterator fi { "data/ingest/xnas-itch-20260831.mbo.csv" };

  OrderBook orderBook {};

  while (!fi.IsEOF()) {
    Trade trade {fi.GetTradeInfo()};
    trade.MakeTrade(orderBook);
  }

  return 0;
}
