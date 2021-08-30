#pragma once

#include <istream>
#include <vector>
#include <bits/types/time_t.h>

namespace bypto::data::binance::klines {

    // Candle data
    struct Kline {
        time_t m_open_time; //start kline time
        long double m_open; // price at @m_open_time
        long double m_high; // highest price between @m_open_time and @m_close_time
        long double m_low; // lowest price between @m_open_time and @m_close_time
        long double m_close; //price at @m_close_time
        long double m_volume; // sum of traded quantity in base asset
        time_t m_close_time; //end kline time
        long double m_quote_asset_volume; // sum of traded quantity in non-base asset
        long m_number_of_trades;
        long double m_taker_buy_base_asset_volume; // How much of @volume was taker buy orders
        long double m_taker_buy_quote_asset_volume; // How much of @quote_asset_volume was taker buy orders
        long double m_ignore;
    };

    std::ostream& operator<<(std::ostream&os,const Kline &kl);

    std::vector<Kline> parseCSV(std::istream &is);

}