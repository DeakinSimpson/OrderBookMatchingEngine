#pragma once

#include <cstdint>
#include <fstream>
#include <ios>
#include <sstream>
#include <string>
#include "orderbook.hpp"
#include "trade.h"

struct RecordHeader
{
    uint8_t length;
    uint8_t rtype;
    uint16_t publisher_id;
    uint32_t instrument_id;
    uint64_t ts_event;
};

struct MboMessage
{
    RecordHeader header;
    uint64_t order_id;
    uint64_t price;
    uint32_t size;
    uint8_t flags;
    uint8_t channel_id;
    char action;
    char side;
    uint64_t ts_recv;
    int32_t ts_in_delta;
    uint32_t sequence;
};

class FileIterator
{
    std::ifstream fs;

public:
    FileIterator(const std::string &filepath)
        : fs{filepath}
    {
        if (!fs.is_open())
        {
            throw std::runtime_error("Could not open file: " + filepath);
        }

        // get the prefix to the file
        char prefix[8];
        fs.read(prefix, sizeof(prefix));

        // read the length of the metadata [0] is version
        const auto metadataLength
                {std::bit_cast<std::array<uint32_t, 2> >(prefix)[1]};

        // start at end of metadata
        fs.seekg(metadataLength, std::ios::cur);
    }

    bool IsEOF() { return fs.eof(); }

    bool Next(MboMessage &out);

    Trade GetTradeInfo(const MboMessage &msg);
};
