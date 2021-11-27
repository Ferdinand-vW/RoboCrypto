#pragma once

#include "bypto/data/price.h"
#include "bypto/order/order.h"
#include "bypto/order/generic.h"
#include "bypto/exchange.h"

#include <binapi/api.hpp>

#include <sstream>

namespace bypto::exchange {
    using namespace data::prices;
    class Binance;

    binapi::rest::api connect_prod_network(boost::asio::io_context &io);
    binapi::rest::api connect_test_network(boost::asio::io_context &io);

    class Binance : public Exchange<Binance,PriceSource::Spot> {
        binapi::rest::api &m_api;

        public:
            Binance(binapi::rest::api &api);
            ~Binance(){};

            Error<int> execute_order(order::GenericOrder go);
            Error<account::Account> get_account_info();
            Error<long double> get_account_value(time_t t = 0);
            Error<long double> fetch_price(Symbol symbol);
            Error<bool> cancel_order(int o_id);
            time_t get_current_time();
            bool tick_once();

            Prices<PriceSource::Spot> get_historical_prices(time_t start,time_t end);
            Error<std::map<Symbol,long double>> get_price_map(time_t t = 0);

            bool is_connected();

            std::stringstream get_funds_csv() const;
            std::stringstream get_pnls_csv() const;
            std::stringstream get_orders_csv() const;
            std::stringstream get_prices_csv() const;
    };

}