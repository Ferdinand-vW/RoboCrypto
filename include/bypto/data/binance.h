#pragma once

#include "bypto/data/klines.h"

#include <istream>
#include <vector>

//interact with binance related data
namespace bypto::data::binance {

    klines::KlineData parseCSV(std::istream &is);
    void prepareTable(common::types::pgconn_t &conn);
    void storeKlines(common::types::pgconn_t &conn,klines::KlineData & klines);
    klines::KlineData loadKlines(common::types::pgconn_t &conn,time_t open_time,time_t close_time);
}