#pragma once

#include "bypto/account.h"
#include "bypto/common/either.h"
#include "bypto/common/types.h"
#include "bypto/data/kline.h"
#include "bypto/data/price.h"
#include "bypto/exchange.h"
#include "bypto/order.h"
#include "bypto/order_type.h"

#include <string>
#include <map>
#include <span>

namespace bypto::exchange {
    using namespace common::types;
    
    template<>
    class Exchange<ExchangeType::BackTest,PriceSource::Kline> {
        data::price::Klines_t m_klines;
        int m_kline_index;

        
        std::map<int,order::Order> m_filled;
        std::map<int,order_type::Partial> m_partials;
        std::map<int,order::Order> m_outstanding;
        account::Account m_account;

        int m_counter = 0;
        Symbol m_symbol;
        time_t m_tick_rate;
        time_t m_curr_time;

        public:
            Exchange(Symbol symbol,Quantity base_fund, Quantity quote_fund,time_t tick_rate,data::price::Klines_t &&klines);

            Error<int> execute_order(order::Order go);
            Error<account::Account> get_account_info();
            Error<Value> get_account_value(time_t t = 0);
            Error<long double> fetch_price(Symbol symbol);
            Error<bool> cancel_order(int o_id);
            Error<bool> tick();
            Error<bool> tick_once();
            std::span<data::price::Kline_t> historical_data(time_t period);
    };
}