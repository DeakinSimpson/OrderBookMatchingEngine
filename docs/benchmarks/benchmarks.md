# Benchmarks
## Benchmark Data by Version
| Version | Improvement |  Type | Average Time (ns) |
| --- | --- | --- | --- |
| 0.1.0 | - None (Initial) | MatchOrders() | 198 |
| 0.2.0 | - OrderPointer Structure | MatchOrders() | 170 |
| 0.3.0 | - CancelOrder<br>- std::list instead of std::vector for orders_ | MatchOrders() | 257 |
| 0.3.0 | - CancelOrder<br>- std::list instead of std::vector for orders_ | CancelOrders() | 426 |

## Raw Benchmark Data
### v0.3.0
```
Run on (12 X 4591.47 MHz CPU s)
CPU Caches:
  L1 Data 32 KiB (x6)
  L1 Instruction 32 KiB (x6)
  L2 Unified 512 KiB (x6)
  L3 Unified 32768 KiB (x1)
Load Average: 1.82, 1.01, 0.90
------------------------------------------------------------------
Benchmark                        Time             CPU   Iterations
------------------------------------------------------------------
BM_MATCHORDERS/10             1899 ns         1889 ns       357122
BM_MATCHORDERS/100           14219 ns        14187 ns        49370
BM_MATCHORDERS/1000         225604 ns       225238 ns         3086
BM_MATCHORDERS/10000       1778954 ns      1774230 ns          411
BM_MATCHORDERS/100000     25856441 ns     25789610 ns           27
BM_MATCHORDERS/1000000   258289029 ns    257758521 ns            3
BM_CANCELORDERS/10            2011 ns         2004 ns       351172
BM_CANCELORDERS/100          16607 ns        16595 ns        41811
BM_CANCELORDERS/1000        225976 ns       225518 ns         3113
BM_CANCELORDERS/10000      2461033 ns      2456905 ns          288
BM_CANCELORDERS/100000    36342918 ns     36222087 ns           20
BM_CANCELORDERS/1000000  427106482 ns    426381703 ns            2
```

### v0.2.0
```
Run on (12 X 3714.39 MHz CPU s)
CPU Caches:
  L1 Data 32 KiB (x6)
  L1 Instruction 32 KiB (x6)
  L2 Unified 512 KiB (x6)
  L3 Unified 32768 KiB (x1)
Load Average: 1.49, 1.09, 0.95
-----------------------------------------------------------------
Benchmark                       Time             CPU   Iterations
-----------------------------------------------------------------
BM_MATCHORDERS/10            1152 ns         1145 ns       607143
BM_MATCHORDERS/100          10163 ns        10140 ns        66838
BM_MATCHORDERS/1000         97971 ns        97899 ns         7166
BM_MATCHORDERS/10000      1029253 ns      1028975 ns          667
BM_MATCHORDERS/100000    13859158 ns     13842511 ns           49
BM_MATCHORDERS/1000000  170580761 ns    170383576 ns            4
```
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