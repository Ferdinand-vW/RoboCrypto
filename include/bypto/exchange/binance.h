#pragma once

#include "bypto/data/price.h"
#include "bypto/exchange.h"
#include "bypto/order/fill.h"
#include "bypto/order/generic.h"
#include "bypto/order/order.h"
#include "bypto/order/order_type.h"

#include <binapi/api.hpp>
#include <tao/pq.hpp>

#include <sstream>

namespace bypto::exchange {
using namespace data::prices;

binapi::rest::api connect_prod_network(boost::asio::io_context &io);
binapi::rest::api connect_test_network(boost::asio::io_context &io);

class Binance : public Exchange<Binance, PriceSource::Spot> {
    binapi::rest::api &m_api;

  private:
    binapi::e_type convert_order_type(order::OType ot) const;
    binapi::e_time convert_time_in_force(order::TimeInForce tif) const;
    binapi::e_side convert_position(order::Position p) const;

    pgconn_t m_database;

    void prepare_tables();
    void sync_price_history(time_t t);
    void sync_order_history(time_t t);
    void sync_account_history(time_t t);

    void write_price_history(time_t t);
    void write_order_history(time_t t);
    void write_account_history(time_t t);

    std::map<time_t, account::Account> m_account_history;
    std::map<time_t, std::tuple<Symbol, long double>> m_price_history;
    std::map<time_t, order::Partial> m_order_history;

    std::map<int, order::GenericOrder> m_orders;
    int m_tick_rate_s;
    int m_total_run_time_s = 0;
    int m_max_run_time_s;

  public:
    Binance(binapi::rest::api &api, int tick_rate_s, int max_run_time_s);
    ~Binance() override{};

    Error<int> execute_order(order::GenericOrder go);
    Error<account::Account> get_account_info();
    Error<long double> get_account_value(time_t t = 0);
    Error<long double> fetch_price(Symbol symbol);
    Error<bool> cancel_order(int o_id);
    time_t get_current_time();
    bool tick_once();

    Prices<PriceSource::Spot> get_historical_prices(time_t start, time_t end);
    Error<std::map<Symbol, long double>> get_price_map(time_t t = 0);

    bool is_connected();

    std::stringstream get_funds_csv() const;
    std::stringstream get_pnls_csv() const;
    std::stringstream get_orders_csv() const;
    std::stringstream get_prices_csv() const;
};

} // namespace bypto::exchange