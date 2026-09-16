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