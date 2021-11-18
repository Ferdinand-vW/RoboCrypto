#pragma once

#include "binapi/api.hpp"
#include "bypto/data/price.h"
#include "bypto/order/order.h"
#include "bypto/order/generic.h"
#include "bypto/exchange.h"

namespace bypto::exchange {
    using namespace data::prices;
    class Binance;

    binapi::rest::api& connect_prod_network(boost::asio::io_context &io);
    binapi::rest::api& connect_test_network(boost::asio::io_context &io);

    class Binance {
        binapi::rest::api &m_api;

        public:
            Binance(binapi::rest::api &api);

            Error<int> execute_order(order::GenericOrder go);
            long double fetch_price(std::string currency);
            account::Account get_account_info();
            void cancel_order(int o_id);
            time_t get_current_time();
            bool tick_once();

            Prices<PriceSource::Spot> get_historical_prices(time_t start,time_t end);

            bool is_connected();
    };

}