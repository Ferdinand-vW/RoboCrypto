#pragma once

#include "bypto/common/types.h"
#include "bypto/common/utils.h"
#include "bypto/data/price.h"
#include "bypto/data/prices.h"

#include <bits/types/time_t.h>
#include <iostream>

namespace bypto::data::price {

    typedef Price<PriceSource::Kline> Kline_t;
    typedef prices::Prices<PriceSource::Kline> Klines_t;
    
    // Candle data
    template<> 
    class Price<PriceSource::Kline> {
        public:
            long double get_price() const { return m_close; }
            long double get_quote_price() const { return 1 / m_close; }
            time_t get_time() const { return m_close_time; }
            common::types::Symbol get_symbol() const { return m_symbol; }
            std::tuple<common::types::Symbol,time_t,long double> as_tuple() const {
                return {get_symbol(),get_time(),get_price()};
            }

            common::types::Symbol m_symbol;
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

            std::string to_string() const;
    };

    common::types::Error<common::utils::time_unit> infer_tick_rate(const std::vector<Kline_t> &klines);

    std::ostream& operator<<(std::ostream&os,const price::Kline_t &kl);


}