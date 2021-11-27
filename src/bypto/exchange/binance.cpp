#include "bypto/exchange/binance.h"

namespace bypto::exchange {

    binapi::rest::api connect_prod_network(boost::asio::io_context& io) {
        throw common::utils::NotImplemented();
        
        const auto pk = std::getenv("BINANCE_PUBLIC_KEY");
        const auto sk = std::getenv("BINANCE_SECRET_KEY");

        binapi::rest::api{
            io
            ,"https://api.binance.com"
            ,"443"
            ,pk // can be empty for non USER_DATA reqs
            ,sk // can be empty for non USER_DATA reqs
            ,10000 // recvWindow
        };
    }

    binapi::rest::api connect_test_network(boost::asio::io_context &io) {
        const auto pk = std::getenv("BINANCE_TEST_PUBLIC_KEY");
        const auto sk = std::getenv("BINANCE_TEST_SECRET_KEY");

        return binapi::rest::api{
            io
            ,"testnet.binance.vision"
            ,"443"
            ,pk // can be empty for non USER_DATA reqs
            ,sk // can be empty for non USER_DATA reqs
            ,10000 // recvWindow
        };
    }

    Binance::Binance(binapi::rest::api &api) : m_api(api) {};

    Error<int> Binance::execute_order(order::GenericOrder go) {

    }

    Error<account::Account> Binance::get_account_info() {

    }

    Error<long double> Binance::get_account_value(time_t t) {

    }

    Error<long double> Binance::fetch_price(Symbol symbol) {

    }

    Error<bool> Binance::cancel_order(int o_id) {

    }

    time_t Binance::get_current_time() {

    }

    bool Binance::tick_once() {

    }

    Prices<PriceSource::Spot> Binance::get_historical_prices(time_t start,time_t end) {

    }

    Error<std::map<Symbol,long double>> Binance::get_price_map(time_t t) {

    }

    bool Binance::is_connected(){

    }

    std::stringstream Binance::get_funds_csv() const {

    }

    std::stringstream Binance::get_pnls_csv() const {

    }

    std::stringstream Binance::get_orders_csv() const {

    }

    std::stringstream Binance::get_prices_csv() const {

    }

}