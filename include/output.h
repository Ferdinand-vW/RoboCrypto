#pragma once

#include <array>
#include <string>

enum class OutputType { Indicator, Fund, PNL, Order, Price };

/**
SPOT
TIME,PRICE

KLINE
.......

-- collect input
-- write input to csv
-- write to csv or stdout

CROSSOVER - MA
-- collect strategy/indicator measurements

-- Account data
-- also responsible by exchange
*/
class OutputWriter {

    std::string m_outdir = "./outdir";
    bool m_write_indicators = false;
    bool m_write_funds = false;
    bool m_write_pnls = false;
    bool m_write_orders = false;
    bool m_write_prices = false;

    void prepare_outdir();
    std::string filepath(std::string filename);

  public:
    OutputWriter();

    void set(OutputType ot);

    void write_indicators(std::stringstream &&ss);
    void write_funds(std::stringstream &&ss);
    void write_pnls(std::stringstream &&ss);
    void write_orders(std::stringstream &&ss);
    void write_prices(std::stringstream &&ss);
};

// write csv can be done with csv::write
// how to get data?

// SPOT/KLINE can be fetched from exchange
// exchanges must be responsible for collecting their data
//  data must be accessible by range of time (i.e start time to current, at most X old)

// strategy data can be fetched from strategy - collector
// data must be accessible by range of time

// account data
// account data must also be accessible by time. Account funds should be stored per tick
// then lookups in spot/kline data can be used to price the account

// how should command flags look like:
//  ./bypto --backtest --dataset=binance/kline/BTCUSDT-15m-2021-07.csv --write-data=funds,orders,indicators,prices,pnl
//