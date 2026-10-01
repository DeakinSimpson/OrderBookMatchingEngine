# Benchmarks
## Benchmark Data by Version
| Version | Improvement |  Type | Average Time (ns) |
| --- | --- | --- | --- |
| 0.1.0 | - None (Initial) | MatchOrders() | 198 |
| 0.2.0 | - OrderPointer Structure | MatchOrders() | 170 |
| 0.3.0 | - CancelOrder<br>- std::list instead of std::vector for orders_ | MatchOrders() | 257 |
| 0.3.0 | - CancelOrder<br>- std::list instead of std::vector for orders_ | CancelOrders() | 426 |
| 0.4.0 | - ModifyOrder Added | ModifyOrder() | 36064 |

## Raw Benchmark Data

### a0.5.0

Update Add Order to use std::prev instead of std::next(orders.begin(), static_cast<ptrdiff_t>(orders.size()) - 1)
Run on (12 X 4641.53 MHz CPU s)
CPU Caches:
L1 Data 32 KiB (x6)
L1 Instruction 32 KiB (x6)
L2 Unified 512 KiB (x6)
L3 Unified 32768 KiB (x1)
Load Average: 0.65, 0.63, 0.62
-----------------------------------------------------------------------------------------
Benchmark                               Time             CPU   Iterations UserCounters...
-----------------------------------------------------------------------------------------
BM_INGEST/iterations:5                210 ms          210 ms            5 bytes_per_second=1.80567Gi/s items_per_second=34.6218M/s
BM_MARKET_REPLAY/iterations:5         970 ms          970 ms            5 items_per_second=7.5048M/s
BM_REPLAY_LATENCY/iterations:1       1181 ms         1181 ms            1 max_ns=5.41024M p50_ns=100 p90_ns=181 p99.9_ns=641 p99_ns=381
BM_ADDORDERS/10                      2430 ns         2430 ns       289253
BM_ADDORDERS/100                    26594 ns        26591 ns        25816
BM_ADDORDERS/1000                  279563 ns       279527 ns         2504
BM_ADDORDERS/10000                3090342 ns      3090051 ns          225
BM_ADDORDERS/100000              41678280 ns     41674959 ns           17
BM_ADDORDERS/1000000            701069450 ns    700986701 ns            1
BM_MATCHORDERS/10                    1792 ns         1789 ns       371911
BM_MATCHORDERS/100                  14169 ns        14161 ns        49564
BM_MATCHORDERS/1000                142802 ns       142786 ns         4943
BM_MATCHORDERS/10000              1469797 ns      1469526 ns          476
BM_MATCHORDERS/100000            21313247 ns     21310405 ns           33
BM_MATCHORDERS/1000000          257331897 ns    257249210 ns            3
BM_CANCELORDERS/10                   1983 ns         1984 ns       352870
BM_CANCELORDERS/100                 16035 ns        16033 ns        43573
BM_CANCELORDERS/1000               205043 ns       205001 ns         3427
BM_CANCELORDERS/10000             2277761 ns      2277528 ns          308
BM_CANCELORDERS/100000           31763725 ns     31761236 ns           22
BM_MODIFYORDERS/10                   3714 ns         3716 ns       188123
BM_MODIFYORDERS/100                 29579 ns        29573 ns        23788
BM_MODIFYORDERS/1000               304491 ns       304473 ns         2260
BM_MODIFYORDERS/10000             3203706 ns      3203472 ns          220
BM_MODIFYORDERS/100000           43854075 ns     43848390 ns           16

Initial
```
Run on (12 X 4641.65 MHz CPU s)
CPU Caches:
  L1 Data 32 KiB (x6)
  L1 Instruction 32 KiB (x6)
  L2 Unified 512 KiB (x6)
  L3 Unified 32768 KiB (x1)
Load Average: 1.62, 0.81, 0.66
-----------------------------------------------------------------------------------------
Benchmark                               Time             CPU   Iterations UserCounters...
-----------------------------------------------------------------------------------------
BM_INGEST/iterations:5                208 ms          207 ms            5 bytes_per_second=1.82981Gi/s items_per_second=35.0846M/s
BM_MARKET_REPLAY/iterations:5        1745 ms         1745 ms            5 items_per_second=4.17073M/s
BM_REPLAY_LATENCY/iterations:1       2007 ms         2007 ms            1 max_ns=8.74082M p50_ns=101 p90_ns=240 p99.9_ns=26.54k p99_ns=1.233k
BM_ADDORDERS/10                      2415 ns         2414 ns       291425
BM_ADDORDERS/100                    26758 ns        26755 ns        26090
BM_ADDORDERS/1000                  277916 ns       277837 ns         2501
BM_ADDORDERS/10000                3082417 ns      3082071 ns          228
BM_ADDORDERS/100000              42278786 ns     42275672 ns           17
BM_ADDORDERS/1000000            684512515 ns    684440228 ns            1
BM_MATCHORDERS/10                    1798 ns         1794 ns       377294
BM_MATCHORDERS/100                  14158 ns        14152 ns        50584
BM_MATCHORDERS/1000                140836 ns       140815 ns         4997
BM_MATCHORDERS/10000              1467512 ns      1467364 ns          474
BM_MATCHORDERS/100000            22378672 ns     22376483 ns           31
BM_MATCHORDERS/1000000          259685533 ns    259628630 ns            3
BM_CANCELORDERS/10                   1965 ns         1964 ns       357784
BM_CANCELORDERS/100                 16336 ns        16329 ns        42859
BM_CANCELORDERS/1000               206568 ns       206508 ns         3416
BM_CANCELORDERS/10000             2291810 ns      2291348 ns          306
BM_CANCELORDERS/100000           33921875 ns     33919482 ns           20
BM_MODIFYORDERS/10                   3711 ns         3717 ns       189127
BM_MODIFYORDERS/100                 30519 ns        30514 ns        23199
BM_MODIFYORDERS/1000               623501 ns       623347 ns         1126
BM_MODIFYORDERS/10000            24983420 ns     24978286 ns           28
BM_MODIFYORDERS/100000         2012970437 ns   2012798638 ns            1
```

### v0.4.0
```
Run on (12 X 3712.04 MHz CPU s)
CPU Caches:
  L1 Data 32 KiB (x6)
  L1 Instruction 32 KiB (x6)
  L2 Unified 512 KiB (x6)
  L3 Unified 32768 KiB (x1)
Load Average: 0.21, 0.47, 0.52
------------------------------------------------------------------
Benchmark                        Time             CPU   Iterations
------------------------------------------------------------------
BM_MATCHORDERS/10             1813 ns         1811 ns       384412
BM_MATCHORDERS/100           14460 ns        14455 ns        49184
BM_MATCHORDERS/1000         219211 ns       218986 ns         3142
BM_MATCHORDERS/10000       1628616 ns      1628511 ns          427
BM_MATCHORDERS/100000     22135169 ns     22132864 ns           32
BM_MATCHORDERS/1000000   256022083 ns    256006551 ns            2
BM_CANCELORDERS/10            1962 ns         1963 ns       357240
BM_CANCELORDERS/100          17097 ns        17094 ns        41022
BM_CANCELORDERS/1000        211513 ns       211503 ns         3295
BM_CANCELORDERS/10000      2369167 ns      2368552 ns          296
BM_CANCELORDERS/100000    31644401 ns     31639014 ns           22
BM_CANCELORDERS/1000000  412319686 ns    412283287 ns            2
BM_MODIFYORDERS/10            3547 ns         3550 ns       197351
BM_MODIFYORDERS/100          31807 ns        31803 ns        22123
BM_MODIFYORDERS/1000        770156 ns       770040 ns          911
BM_MODIFYORDERS/10000     39776892 ns     39774996 ns           18
BM_MODIFYORDERS/100000  3606814616 ns   3606448754 ns            1
```
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