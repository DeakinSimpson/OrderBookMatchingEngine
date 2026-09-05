# Benchmarks
## Benchmark Data by Version
| Version | Improvement |  Type | Average Time (ns) |
| --- | --- | --- | --- |
| 0.1.0 | None (Initial) | MatchOrders() | 133 |

## Raw Benchmark Data
### v0.1.0
```
Run on (12 X 4641.18 MHz CPU s)
CPU Caches:
L1 Data 32 KiB (x6)
L1 Instruction 32 KiB (x6)
L2 Unified 512 KiB (x6)
L3 Unified 32768 KiB (x1)
Load Average: 0.96, 0.71, 0.44
-----------------------------------------------------------------
Benchmark                       Time             CPU   Iterations
-----------------------------------------------------------------
BM_MATCHORDERS/10             958 ns          960 ns       725017
BM_MATCHORDERS/100           6235 ns         6244 ns       110296
BM_MATCHORDERS/1000         61955 ns        61945 ns        11266
BM_MATCHORDERS/10000       765959 ns       765924 ns          876
BM_MATCHORDERS/100000    10058590 ns     10057819 ns           68
BM_MATCHORDERS/1000000  133998258 ns    133988507 ns            5
```