# Benchmarks
## Benchmark Data by Version
| Version | Improvement |  Type | Average Time (ns) |
| --- | --- | --- | --- |
| 0.1.0 | None (Initial) | MatchOrders() | 198 |

## Raw Benchmark Data
###

### v0.1.0
```
Run on (12 X 4639.88 MHz CPU s)
CPU Caches:
  L1 Data 32 KiB (x6)
  L1 Instruction 32 KiB (x6)
  L2 Unified 512 KiB (x6)
  L3 Unified 32768 KiB (x1)
Load Average: 1.97, 1.16, 0.95
-----------------------------------------------------------------
Benchmark                       Time             CPU   Iterations
-----------------------------------------------------------------
BM_MATCHORDERS/10             948 ns          944 ns       740690
BM_MATCHORDERS/100           6035 ns         6016 ns       116061
BM_MATCHORDERS/1000         59462 ns        59262 ns        11876
BM_MATCHORDERS/10000      1289780 ns      1286383 ns          538
BM_MATCHORDERS/100000    16039431 ns     15982324 ns           44
BM_MATCHORDERS/1000000  198058344 ns    197204332 ns            4
```