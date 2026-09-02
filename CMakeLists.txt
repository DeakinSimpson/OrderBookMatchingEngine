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
