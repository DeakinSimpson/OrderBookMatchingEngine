cmake_minimum_required(VERSION 3.23)

project(OBME)

set(CMAKE_CXX_STANDARD, 26)
set(CMAKE_CXX_STANDARD_REQUIRED, ON)

add_executable(OBME 
  src/main.cpp
)

target_include_directories(OBME PRIVATE include)

target_compile_options(OBME PRIVATE 
  -Wall -Wextra -Weffc++ -Wconversion 
  -Wsign-conversion -Werror -pedantic-errors -ggdb
)

# add the google benchmark
include(FetchContent)
FetchContent_Declare(
        googlebenchmark
        GIT_REPOSITORY https://github.com/google/benchmark.git
        GIT_TAG v1.9.1
)
# ignore googles own testing
set(BENCHMARK_ENABLE_TESTING OFF CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googlebenchmark)

add_executable(orderbook_bench
        benchmarks/orderbook_benchmark.cpp)
target_include_directories(orderbook_bench PRIVATE include)
target_link_libraries(orderbook_bench PRIVATE benchmark::benchmark)
