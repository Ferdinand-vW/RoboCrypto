#pragma once

#include "bypto/data/price.h"
#include "bypto/data/prices.h"
#include "bypto/order/order.h"
#include "bypto/account.h"
#include "bypto/common/types.h"

#include <span>

namespace bypto::exchange {
    using namespace data::price;
    using namespace common::types;

    enum class ExchangeType { BackTest, Binance };

    template<typename E,PriceSource P>
    class Exchange {
        public:
            template<typename OT>
            Error<int> execute_order(order::Order<OT> order) {
                return static_cast<E*>(this)->execute_order(order);
            }

            Error<account::Account> get_account_info() {
                return static_cast<E*>(this)->get_account_info();
            }

            Error<long double> get_account_value(time_t t = 0) {
                return static_cast<E*>(this)->get_account_value(t);
            }

            Error<long double> fetch_price() {
                return static_cast<E*>(this)->fetch_price();
            }

            Error<bool> cancel_order(int o_id) {
                return static_cast<E*>(this)->cancel_order(o_id);
            }

            time_t get_current_time() {
                return static_cast<E*>(this)->get_current_time();
            }

            Error<bool> tick_once() {
                return static_cast<E*>(this)->tick_once();
            }

            data::prices::Prices<P> get_historical_prices(time_t start,time_t end) {
                return static_cast<E*>(this)->get_historical_prices(start,end);
            }

            Error<std::map<Symbol,long double>> get_price_map(time_t t = 0) {
                return static_cast<E*>(this)->get_price_map(t);
            }
    };

}