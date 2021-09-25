#pragma once

#include "bypto/data/binance/klines.h"
#include "bypto/exchange.h"
#include "bypto/order.h"
#include "bypto/order_type.h"

#include <string>

namespace bypto::exchange::back_testing {

    class BackTesting : public Exchange {
        std::deque<data::binance::klines::Kline> m_klines;
        std::map<int,order::Order> m_filled;
        std::map<int,order_type::Partial> m_partials;
        std::map<int,order::Order> m_outstanding;
        int m_counter = 0;
        std::string m_symbol;
        int m_tick_rate;
        time_t m_curr_time;

        public:
            BackTesting(std::string symbol,int tick_rate,std::deque<data::binance::klines::Kline> klines);

            int execute_order(order::Order go);
            long double fetch_price();
            void cancel_order(int o_id);
            bool tick_once();
    };
}