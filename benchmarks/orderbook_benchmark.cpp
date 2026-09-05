//
// Created by DeakinSimpson on 9/5/26.
//
#include <benchmark/benchmark.h>
#include "orderbook.hpp"

/*
 * Benchmark to test the MatchOrders() function
 */
static void BM_MATCHORDERS(benchmark::State& state) {
  // get the number of orders as the range of the input state
  const int numOrders { static_cast<int>(state.range(0)) };

  // loop through from 0-maxrange of the input state
  for (auto _ : state) {
    // pause the timing for the setup
    state.PauseTiming();

    OrderBook BMOrderBook {};

    // create bids from 100 -> 100 + numOrders -1
    for (int i {}; i < numOrders; ++i) {
      BMOrderBook.AddOrder({
        static_cast<OrderId>(i),
        Side::Bid,
        static_cast<Price>(100 + i),
        10
      });
    }

    // create asks from 100 -> 100 + numOrders -1
    for (int i {}; i < numOrders; ++i) {
      BMOrderBook.AddOrder({
        static_cast<OrderId>(numOrders + i),
        Side::Ask,
        static_cast<Price>(100 + i),
        10
      });
    }

    // NOTE: Every order will be filled as they are identical bids and asks

    // start timing again
    state.ResumeTiming();

    // time the match orders
    BMOrderBook.MatchOrders();
  }
}

// test different benchmark ranges
BENCHMARK(BM_MATCHORDERS)
  -> Arg(10)
  -> Arg(100)
  -> Arg(1000)
  -> Arg(10000)
  -> Arg(100000)
  -> Arg(1000000);

BENCHMARK_MAIN();


