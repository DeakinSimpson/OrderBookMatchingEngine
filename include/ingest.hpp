#pragma once

#include <cstdint>
#include <fstream>
#include <ios>
#include <random>
#include <sstream>
#include <string>
#include "orderbook.hpp"
#include "trade.hpp"

struct RecordHeader
{
    uint8_t     length;
    uint8_t     rtype;
    uint16_t    publisher_id;
    uint32_t    instrument_id;
    uint64_t    ts_event;
};

struct MboMessage
{
    RecordHeader    header;
    uint64_t        order_id;
    uint64_t        price;
    uint32_t        size;
    uint8_t         flags;
    uint8_t         channel_id;
    char            action;
    char            side;
    uint64_t        ts_recv;
    int32_t         ts_in_delta;
    uint32_t        sequence;
};

class FileIterator
{
    std::ifstream fs;
    std::jthread ingestThread_ {};

public:
    FileIterator(const std::string& filepath, TSC::Queue<Trade>& trades,
        std::atomic<bool>& ingestComplete)

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

        /* initialise the ingest thread */
        ingestThread_ = std::jthread([this, &trades, &ingestComplete]()
            {Thread(trades, ingestComplete);});
    }

    // the code that will be run on the ingest thread
    void Thread(TSC::Queue<Trade> &trades, std::atomic<bool>& isComplete);

    bool IsEOF() { return fs.eof(); }

    bool Next(MboMessage &out);

    Trade GetTradeInfo(const MboMessage &msg);
};

class SyntheticIngest
{
    /* Default values */
    const Quantity MAX_QUANTITY { 1000 };
    const Price MIN_PRICE_DOLLARS { 100 };
    const Price MAX_PRICE_DOLLARS { 1000 };
    const Price ONE_DOLLAR { 1'000'000'000 };
    const double PRICE_MEAN_DOLLARS { 550.0 };
    const double PRICE_STDDEV_DOLLARS { 150.0 };

    /* define random number generator variables */
    std::mt19937 gen_;
    std::normal_distribution<double> price_distribution_;
    std::uniform_int_distribution<OrderId> id_distribution_;
    std::uniform_int_distribution<Quantity> qty_distribution_;
    std::uniform_int_distribution<int> side_distribution_;
    std::uniform_int_distribution<int> type_distribution_;
    std::jthread ingest_thread_;

public:
    SyntheticIngest(const uint64_t num_trades,
        TSC::Queue<Trade>& trades, std::atomic<bool>& ingestComplete)
        : gen_{ std::random_device{}() }
        , price_distribution_{ PRICE_MEAN_DOLLARS, PRICE_STDDEV_DOLLARS }
        , id_distribution_{ 1, std::numeric_limits<OrderId>::max() }
        , qty_distribution_{ 1, MAX_QUANTITY }
        , side_distribution_{ 0, 1 }
        , type_distribution_{ 0, 2 }
        , ingest_thread_{ [this ,num_trades, &trades, &ingestComplete]() {
            Thread(num_trades, trades, ingestComplete);
        }}
    {   }

private:
    void Thread(uint64_t num_trades, TSC::Queue<Trade>& trades,
        std::atomic<bool>& ingestComplete)
    {
        while (num_trades > 0)
        {
            Trade trade { GenRandTrade() };

            /* thread spin is fine here */
            while (!trades.push(trade)) { ; }

            --num_trades;
        }

        ingestComplete.store(true);
    }

    Price GenRandPrice()
    {
        /* if the price is over MAX_PRICE redo */
        for (;;)
        {
            const double dollars { std::round(price_distribution_(gen_)) };

            if (dollars >= static_cast<double>(MIN_PRICE_DOLLARS)
                && dollars <= static_cast<double>(MAX_PRICE_DOLLARS))
            {
                return static_cast<Price>(dollars) * ONE_DOLLAR;
            }
        }
    }
    Side GenRandSide()
    {
        return (side_distribution_(gen_) == 0) ? Side::Ask : Side::Bid;
    }
    Quantity GenRandQuantity()
    {
        return qty_distribution_(gen_);
    }
    OrderId GenRandOrderId()
    {
        return id_distribution_(gen_);
    }
    TradeType GenRandTradeType()
    {
        switch (type_distribution_(gen_))
        {
            case 0:
                return TradeType::Add;
            case 1:
                return TradeType::Cancel;
            case 2:
                return TradeType::Modify;
            default:
                return TradeType::None;
        }
    }
    Trade GenRandTrade()
    {
        return TradeInfo {
            GenRandOrderId(),
            GenRandSide(),
            GenRandPrice(),
            GenRandQuantity(),
            GenRandTradeType()
        };
    }
};