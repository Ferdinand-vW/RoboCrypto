#pragma once

#include "bypto/common/either.h"
#include "bypto/data/klines.h"
#include "bypto/data/price.h"
#include "bypto/exchange.h"
#include "bypto/order.h"
#include "bypto/order_type.h"

#include <string>
#include <map>
#include <span>

namespace bypto::exchange {
    using namespace common::types;
    using namespace data::klines;

    class BackTest;

    template<>
    class Exchange<BackTest,PriceSource::Kline> {
        KlineData m_klines;
        int m_kline_index;
        
        std::map<int,order::Order> m_filled;
        std::map<int,order_type::Partial> m_partials;
        std::map<int,order::Order> m_outstanding;
        int m_counter = 0;
        std::string m_symbol;
        int m_tick_rate;
        time_t m_curr_time;

        public:
            Exchange(std::string symbol,int tick_rate,KlineData &&klines);

            Error<int> execute_order(order::Order go);
            Error<long double> fetch_price(std::string symbol);
            Error<bool> cancel_order(int o_id);
            Error<bool> tick();
            Error<bool> tick_once();
            std::span<data::price::Kline_t> historical_data(time_t period);
    };
}