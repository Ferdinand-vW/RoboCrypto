#pragma once

#include "bypto/common/types.h"
#include "bypto/data/kline.h"
#include "bypto/data/price.h"

#include <istream>
#include <vector>

//interact with binance related data
namespace bypto::data::binance {

    price::Klines_t parseCSV(common::types::Symbol symbol,std::istream &is);
    void prepareTable(common::types::pgconn_t &conn);
    void storeKlines(common::types::pgconn_t &conn,price::Klines_t & klines);
    price::Klines_t loadKlines(common::types::pgconn_t &conn,time_t open_time,time_t close_time);
}