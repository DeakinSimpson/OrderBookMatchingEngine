cmake_minimum_required(VERSION 3.23)

project(OBME)

set(CMAKE_CXX_STANDARD 26)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(OBME 
  src/main.cpp
)

target_include_directories(OBME PRIVATE include)

target_compile_options(OBME PRIVATE 
  -Wall -Wextra -Weffc++ -Wconversion 
  -Wsign-conversion -Werror -pedantic-errors -ggdb
)

# --- add the google benchmark ---
# get the content
include(FetchContent)
FetchContent_Declare(
        googlebenchmark
        GIT_REPOSITORY https://github.com/google/benchmark.git
        GIT_TAG v1.9.1
)
FetchContent_Declare(
        googletest
        URL https://github.com/google/googletest/archive/refs/tags/v1.15.2.zip)

# ignore googles own testing
set(BENCHMARK_ENABLE_TESTING OFF CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googlebenchmark googletest)

add_executable(engine_bench
        benchmarks/orderbook_benchmark.cpp)
target_include_directories(engine_bench PRIVATE include)
target_link_libraries(engine_bench PRIVATE benchmark::benchmark)

# --- add google testing ---
add_executable(engine_tests
        tests/orderbook_test.cpp)
target_include_directories(engine_tests PRIVATE include)
target_link_libraries(engine_tests gtest_main)

