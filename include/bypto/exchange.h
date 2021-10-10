#pragma once

#include "bypto/data/price.h"
#include "bypto/data/prices.h"
#include "bypto/order.h"
#include "bypto/account.h"
#include "bypto/common/types.h"

#include <span>

namespace bypto::exchange {
    using namespace data::price;
    using namespace common::types;

    enum class ExchangeType { BackTest, Binance };

    template<ExchangeType T,PriceSource P>
    class Exchange {
        public:
            Error<int> execute_order(order::Order order);
            Error<account::Account> get_account_info();
            Error<long double> get_account_value(time_t t = 0);
            Error<long double> fetch_price();
            Error<bool> cancel_order(int o_id);
            Error<bool> tick_once();
            data::prices::Prices<P>& historical_data(time_t start,time_t end);
    };

    typedef Exchange<ExchangeType::BackTest,PriceSource::Kline> BackTestExchange;
}