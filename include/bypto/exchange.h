#pragma once

#include "bypto/data/price.h"
#include "bypto/order.h"
#include "bypto/account.h"
#include "bypto/common/types.h"

#include <span>

namespace bypto::exchange {
    using namespace data::price;
    using namespace common::types;

    template<typename T,PriceSource P>
    class Exchange {
        public:
            Error<int> execute_order(order::Order order);
            Error<account::Account> get_account_info();
            Error<long double> fetch_price();
            Error<bool> cancel_order(int o_id);
            Error<bool> tick_once();
            std::span<data::price::Price<P>> historical_data(time_t start,time_t end);
    };
}