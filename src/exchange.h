#pragma once

#include "bypto/exchange.h"
#include "bypto/exchange/binance.h"
#include "bypto/exchange/backtest.h"
#include "bypto/data/price.h"
#include "command.h"

#include <map>
#include <string>
#include <fstream>
#include <memory>

using namespace bypto::indicator;
using namespace bypto::strategy;
using namespace bypto::exchange;

BackTest backtest(const CommandOptions &opts) {
    //parse binance klines historical data
    std::fstream fs("/home/ferdinand/dev/bypto/historical/binance/kline/BTCUSDT-15m-2021-07.csv");
    auto klines = parseCSV(opts.m_sym,fs);
    
    auto conn = tao::pq::connection::create("dbname=historical");
    prepareTable(conn);
    storeKlines(conn,klines);

    //Start with 1 BTC and 1000 USDT
    BackTestParams btp{opts.m_sym,1,1000,std::nullopt,std::nullopt,std::move(klines)};
    
    return BackTest(std::move(btp));
}

Binance binance(const CommandOptions &opts,binapi::rest::api &api) {
    return Binance(api,opts.m_tick_rate_s,opts.m_max_run_time_s);
}

Binance binance_test(const CommandOptions &opts,binapi::rest::api &api) {
    return Binance(api,opts.m_tick_rate_s,opts.m_max_run_time_s);
}