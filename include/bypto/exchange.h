#pragma once

#include "bypto/data/price.h"
#include "bypto/data/prices.h"
#include "bypto/order/order.h"
#include "bypto/account.h"
#include "bypto/common/types.h"

#include <span>
#include <string>
#include <sstream>

namespace bypto::exchange {
    using namespace data::price;
    using namespace common::types;

    enum class TagExchange { BackTest, Binance, BinanceTest };

    template<typename E,PriceSource P>
    class Exchange {
        public:
            virtual ~Exchange(){};

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

            std::stringstream get_funds_csv() const {
                return static_cast<const E*>(this)->get_funds_csv();
            }
            std::stringstream get_pnls_csv() const {
                return static_cast<const E*>(this)->get_pnls_csv();
            }
            std::stringstream get_orders_csv() const {
                return static_cast<const E*>(this)->get_orders_csv();
            }
            std::stringstream get_prices_csv() const {
                return static_cast<const E*>(this)->get_prices_csv();
            }
    };

}