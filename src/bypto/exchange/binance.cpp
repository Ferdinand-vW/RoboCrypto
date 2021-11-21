#include "bypto/exchange/binance.h"

namespace bypto::exchange {

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

}