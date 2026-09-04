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

  Order GetOrder() {
    // get the next line
    std::string line;
    std::getline(fs, line);
    
    // convert the line string to a string stream
    std::stringstream lineStream(line);
    std::string cell;
    
    std::getline(lineStream, cell, ',');
    uint64_t ts_recv { std::stoull(cell) };
    std::getline(lineStream, cell, ','); // skip ts_event
    std::getline(lineStream, cell, ','); // skip rtype
    std::getline(lineStream, cell, ','); // skip publisher_id                           
    std::getline(lineStream, cell, ','); // skip instument_id
    std::getline(lineStream, cell, ','); 
    // char action { cell[0] };
    std::getline(lineStream, cell, ',');
    char side { cell[0] };
    std::getline(lineStream, cell, ',');
    int64_t price { std::stoll(cell) };
    std::getline(lineStream, cell, ',');
    uint32_t size { static_cast<uint32_t>(std::stoul(cell)) };
    std::getline(lineStream, cell, ','); // skip channel_id
    std::getline(lineStream, cell, ','); 
    uint64_t order_id { std::stoull(cell) }; 
    std::getline(lineStream, cell, ','); // skip flags
    std::getline(lineStream, cell, ','); // skip ts_in_delta
    std::getline(lineStream, cell, ','); // skip sequence
    
    Side side_ {};

    if (side == 'A') {
      side_ = Side::Ask;
    } else if (side == 'B') {
      side_ = Side::Bid;
    }

    return { order_id, side_, price, size, ts_recv};
  } 

private:
  std::ifstream fs;
};
