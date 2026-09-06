# OrderBookMatchingEngine
This is a market by order orderbook matching engine. It takes in orders, inputs them into an
orderbook and then performs order matching to match bids and asks. It uses First in First Price-Time
priority, meaning, that the best bidder or asker is prioritized and then the time that the order came
in is used to break the tie, in the common event that 2 or more orders have the same bid or ask price.

## How to Run:
To Run the project we start by building:
```
cmake -B cmake-build-release
cmake --build cmake-build-release
```
Then we run it, this project inputs .mbo (Market By Order) data using csv format, we must input
the market by order data as an argument:
```
./cmake-build-release/OBME path/to/file.mbo.csv
```
## How to Benchmark
First we must build the project:
```
cmake -B cmake-build-release
cmake --build cmake-build-release
```
To Run a benchmark we simply use the following command:
```
./cmake-build-release/orderbook-bench
```
## How to Test
To test, we first must build the project:
```
cmake -B cmake-build-release
cmake --build cmake-build-release
```
Then we can simply run the ctest
```
ctest --test-dir cmake-build-release
```
## Information on the Project
### Benchmarks
To find the parented most up-to-date benchmarks as well as all previous benchmarks [view here](./docs/benchmarks/benchmarks.md)

### Design Decisions
For a history of design decisions and why they were made [view here](./docs/design.md)
