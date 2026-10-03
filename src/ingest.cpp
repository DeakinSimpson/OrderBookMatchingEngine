//
// Created by deakin on 9/8/26.
//

#include "ingest.hpp"
#include "trade.h"

bool FileIterator::Next(MboMessage &out)
{
    constexpr uint8_t RTYPE_MBO{0xA0};
    constexpr size_t HEADER_SIZE{sizeof(RecordHeader)};
    constexpr size_t MESSAGE_SIZE{sizeof(MboMessage) - HEADER_SIZE};

    // read the header into out.header
    while (fs.read(reinterpret_cast<char *>(&out.header), HEADER_SIZE))
    {
        // check if the order is a MBO order
        if (out.header.rtype != RTYPE_MBO)
        {
            // jump the headers length - the header that we have passed
            fs.seekg(out.header.length * 4 - static_cast<long int>(HEADER_SIZE), std::ios::cur);
            continue;
        }

        // read from end of head to message size into out
        fs.read(reinterpret_cast<char *>(&out) + HEADER_SIZE, MESSAGE_SIZE);
        return true;
    }

    // EOF
    return false;
}

static Side ToSide(const char c)
{
    switch (c)
    {
        case 'A': return Side::Ask;
        case 'B': return Side::Bid;
        default: return Side::None;
    }
}

static TradeType ToTradeType(const char c)
{
    switch (c)
    {
        case 'A': return TradeType::Add;
        case 'C': return TradeType::Cancel;
        case 'M': return TradeType::Modify;
        default: return TradeType::None;
    }
}

Trade FileIterator::GetTradeInfo(const MboMessage &msg)
{
    return Trade{
        TradeInfo{
            msg.order_id,
            ToSide(msg.side),
            msg.price,
            msg.size,
            ToTradeType(msg.action)
        }
    };
}
