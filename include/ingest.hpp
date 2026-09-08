#pragma once

#include <cstdint>
#include <fstream>
#include <ios>
#include <limits>
#include <sstream>
#include <string>
#include "orderbook.hpp"

class FileIterator {
public:
  FileIterator(const std::string& filepath)
    : fs{filepath}
  { 
    // skip header
    fs.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

    // skip orderbook reset
    fs.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
  }

  bool IsEOF() { return fs.eof(); }

  TradeInfo GetTradeInfo();

private:
  std::ifstream fs;
};
