#pragma once

#include "bypto/account.h"
#include "bypto/common/either.h"
#include "bypto/common/types.h"
#include "bypto/common/utils.h"
#include "bypto/data/binance.h"
#include "bypto/data/kline.h"
#include "bypto/data/price.h"
#include "bypto/exchange.h"
#include "bypto/order/order.h"
#include "bypto/order/generic.h"
#include "bypto/order/order_type.h"
#include "bypto/order/fill.h"

#include <string>
#include <map>
#include <span>

namespace bypto::exchange {
    using namespace common::types;
    using namespace common::utils;
    using namespace bypto::data::binance;
    
    struct BackTestParams {
        //Support for backtesting is limited to one currencypair at a time
        Symbol m_sym;
        //initial funds
        Quantity m_base_qty;
        Quantity m_quote_qty;
        //start time defaults to first kline in @m_klines@
        std::optional<time_t> m_start_time;
        //tick rate defaults to tick rate in @m_klines@
        std::optional<time_unit> m_tick_rate;
        //historical kline data used for back testing
        std::vector<Kline_t> m_klines;
    };

    class BackTest : public Exchange<BackTest,PriceSource::Kline> {
        std::vector<Kline_t> m_klines;
        int m_kline_index = 0;

        
        std::map<int,order::GenericOrder> m_filled;
        std::map<int,order::GenericOrder> m_outstanding;
        account::Account m_account;

        int m_counter = 0;
        Symbol m_symbol;
        time_unit m_tick_rate;
        time_t m_curr_time;

        void update_account(order::Partial p);

        public:
            BackTest(BackTestParams &&prms);
            BackTest(const BackTest &) = delete;
            BackTest& operator=(const BackTest &) = delete;
            ~BackTest(){};

            template<typename T>
            Error<int> execute_order(order::Order<T> go) {
                m_outstanding.insert({m_counter,order::GenericOrder(go)});
                m_counter++;
                return m_counter - 1;
            }

            //Return prices for symbols as given time
            Error<std::map<Symbol,long double>> get_price_map(time_t t = 0);

            Error<account::Account> get_account_info();
            Error<long double> fetch_price(Symbol symbol);
            Error<bool> cancel_order(int o_id);
            Error<bool> tick_once();
            time_t get_current_time();
            Klines_t get_historical_prices(time_t start,time_t end);
            Klines_t get_all_historical();
    };
}