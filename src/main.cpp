#include <chrono>
#include <iostream>
#include "orderbook.hpp"
#include "ingest.hpp"
int main () {
  FileIterator fi { "data/ingest/xnas-itch-20260831.mbo.csv" };

  TradeInfo tradeInfoTest {fi.GetOrder()};

  std::cout << tradeInfoTest.orderID << std::endl;

  return 0;
}
