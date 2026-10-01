//
// Created by DeakinSimpson on 9/5/26.
//
#include <chrono>
#include <benchmark/benchmark.h>

#include "ingest.hpp"
#include "orderbook.hpp"


// benchmark for ingest speed
static void BM_INGEST(benchmark::State& state)
{
    int64_t messages {};

    for (auto _ : state)
    {
        FileIterator fi { DATA_FILE };
        MboMessage msg;

        while (fi.Next(msg))
        {
            // this loop does nothing with message, DoNotOptimize means the
            // compiler wont compole away msg
            benchmark::DoNotOptimize(msg);
            ++messages;
        }
    }

    // items per second
    state.SetItemsProcessed(messages);

    // bytes per second
    state.SetBytesProcessed(static_cast<int64_t>(messages * sizeof(MboMessage)));
}

// run INGEST benchmark
BENCHMARK(BM_INGEST)->Unit(benchmark::kMillisecond)->Iterations(5);

// benchmark for replay speed
static void BM_MARKET_REPLAY(benchmark::State& state)
{
    int64_t messages {};

    for (auto _ : state)
    {
        FileIterator fi { DATA_FILE };
        MboMessage msg;
        OrderBook orderbook {1000000};

        while (fi.Next(msg))
        {
            fi.GetTradeInfo(msg).MakeTrade(orderbook);
            ++messages;
        }
    }

    // items per second
    state.SetItemsProcessed(messages);
}
BENCHMARK(BM_MARKET_REPLAY)->Unit(benchmark::kMillisecond)->Iterations(5);

static void BM_REPLAY_LATENCY(benchmark::State& state)
{
    using Clock = std::chrono::steady_clock;

    std::vector<int64_t> timeSamples;
    timeSamples.reserve(10'000'000); // reserve 10M samples

    for (auto _ : state)
    {
        FileIterator fi { DATA_FILE };
        OrderBook orderBook {1000000};
        MboMessage msg;

        while (fi.Next(msg))
        {
            const auto start { Clock::now() };
            fi.GetTradeInfo(msg).MakeTrade(orderBook);
            const auto end { Clock::now() };

            // push back time it took to make the trade in the orderbook
            timeSamples.push_back((end - start).count());
        }
    }

    std::ranges::sort(timeSamples);
    const auto percentile = [&](const double percentile)
    {
        // get the index at the percentile
        double sizeAsDouble { static_cast<double>(timeSamples.size() - 1) };
        size_t index { static_cast<size_t>(percentile * sizeAsDouble) };

        // return time sample as a double
        return static_cast<double>(timeSamples[index]);
    };

    state.counters["p50_ns"]   = percentile(0.50);
    state.counters["p90_ns"]   = percentile(0.90);
    state.counters["p99_ns"]   = percentile(0.99);
    state.counters["p99.9_ns"] = percentile(0.999);
    state.counters["max_ns"]   = static_cast<double>(timeSamples.back());
}

BENCHMARK(BM_REPLAY_LATENCY)->Unit(benchmark::kMillisecond)->Iterations(1);

static void BM_ADDORDERS(benchmark::State& state) {
    // get the number of orders as the range of the input state
    const int numOrders { static_cast<int>(state.range(0)) };

    // loop through from 0-maxrange of the input state
    for (auto _ : state) {
        OrderBook BMOrderBook {};

        // create bids from 100 -> 100 + numOrders -1
        for (int i {}; i < numOrders; ++i) {
            BMOrderBook.AddOrder(std::make_shared<Order>(Order{
              static_cast<OrderId>(i),
              Side::Bid,
              static_cast<Price>(100 + i),
              10
            }));
        }

        // create asks from 100 -> 100 + numOrders -1
        for (int i {}; i < numOrders; ++i) {
            BMOrderBook.AddOrder(std::make_shared<Order>(Order{
              static_cast<OrderId>(numOrders + i),
              Side::Ask,
              static_cast<Price>(100 + i),
              10
            }));
        }
    }
}

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
      BMOrderBook.AddOrder(std::make_shared<Order>(Order{
        static_cast<OrderId>(i),
        Side::Bid,
        static_cast<Price>(100 + i),
        10
      }));
    }

    // create asks from 100 -> 100 + numOrders -1
    for (int i {}; i < numOrders; ++i) {
      BMOrderBook.AddOrder(std::make_shared<Order>(Order{
        static_cast<OrderId>(numOrders + i),
        Side::Ask,
        static_cast<Price>(100 + i),
        10
      }));
    }

    // NOTE: Every order will be filled as they are identical bids and asks

    // start timing again
    state.ResumeTiming();

    // time the match orders
    BMOrderBook.MatchOrders();
  }
}

/*
 * Benchmark to test the MatchOrders() function
 */
static void BM_CANCELORDERS(benchmark::State& state) {
  // get the number of orders as the range of the input state
  const int numOrders { static_cast<int>(state.range(0)) };

  // loop through from 0-maxrange of the input state
  for (auto _ : state) {
    // pause the timing for the setup
    state.PauseTiming();

    OrderBook BMOrderBook {};

    for (int i {}; i < numOrders; ++i) {
      BMOrderBook.AddOrder(std::make_shared<Order>(Order{
        static_cast<OrderId>(i),
        Side::Bid,
        static_cast<Price>(100 + i),
        10
      }));
    }

    for (int i {}; i < numOrders; ++i) {
      BMOrderBook.AddOrder(std::make_shared<Order>(Order{
        static_cast<OrderId>(numOrders + i),
        Side::Ask,
        static_cast<Price>(100 + i),
        10
      }));
    }

    // start timing again
    state.ResumeTiming();

    for (int i {}; i < numOrders * 2; ++i) {
      // get a mix between underfill and overfill
      BMOrderBook.CancelOrder(i, (9 + (i % 3)));
    }
  }
}

/*
 * A benchmark for ModifyOrder()
 */
static void BM_MODIFYORDERS(benchmark::State& state) {
  // get the number of orders as the range of the input state
  const int numOrders { static_cast<int>(state.range(0)) };

  // loop through from 0-maxrange of the input state
  for (auto _ : state) {
    // pause the timing for the setup
    state.PauseTiming();

    OrderBook BMOrderBook {};

    for (int i {}; i < numOrders; ++i) {
      BMOrderBook.AddOrder(std::make_shared<Order>(Order{
        static_cast<OrderId>(i),
        Side::Bid,
        static_cast<Price>(100 + i),
        10
      }));
    }

    for (int i {}; i < numOrders; ++i) {
      BMOrderBook.AddOrder(std::make_shared<Order>(Order{
        static_cast<OrderId>(numOrders + i),
        Side::Ask,
        static_cast<Price>(100 + i),
        10
      }));
    }

    // start timing again
    state.ResumeTiming();

    for (int i {}; i < numOrders * 2; ++i) {
      // get a mix between underfill and overfill
      BMOrderBook.ModifyOrder(i, (9 + (i % 3)), (9 + (i % 3)));
    }
  }
}

// test different benchmark ranges
BENCHMARK(BM_ADDORDERS)
  -> Arg(10)
  -> Arg(100)
  -> Arg(1000)
  -> Arg(10000)
  -> Arg(100000)
  -> Arg(1000000);

BENCHMARK(BM_MATCHORDERS)
  -> Arg(10)
  -> Arg(100)
  -> Arg(1000)
  -> Arg(10000)
  -> Arg(100000)
  -> Arg(1000000);

BENCHMARK(BM_CANCELORDERS)
  -> Arg(10)
  -> Arg(100)
  -> Arg(1000)
  -> Arg(10000)
  -> Arg(100000);

BENCHMARK(BM_MODIFYORDERS)
  -> Arg(10)
  -> Arg(100)
  -> Arg(1000)
  -> Arg(10000)
  -> Arg(100000);


BENCHMARK_MAIN();


