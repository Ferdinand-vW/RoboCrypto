#include "bypto/exchange/binance.h"
#include "bypto/common/utils.h"
#include "bypto/order/types.h"
#include "bypto/common/std.h"

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

    binapi::e_type Binance::convert_order_type(order::OType ot) const {
        using namespace order;
        switch(ot) {
            case OType::Unit: 
                std::invalid_argument("Cannot convert OType::Unit to binapi::e_type");
                return binapi::e_type::market; // won't ever get here
            break; case OType::Market: return binapi::e_type::market;
            break; case OType::Limit: return binapi::e_type::limit;
            break; case OType::StopLoss: return binapi::e_type::stop_loss;
            break; case OType::StopLossLimit: return binapi::e_type::stop_loss_limit;
            break; case OType::TakeProfit: return binapi::e_type::take_profit;
            break; case OType::TakeProfitLimit: return binapi::e_type::take_profit_limit;
            break; case OType::LimitMaker: return binapi::e_type::limit_maker;
        }
    }

    binapi::e_time Binance::convert_time_in_force(order::TimeInForce tif) const { 
        using namespace order;
        switch(tif) {
            case TimeInForce::FOK: return binapi::e_time::FOK;
            case TimeInForce::GTC: return binapi::e_time::GTC;
            case TimeInForce::IOC: return binapi::e_time::IOC;
        }
    }

    binapi::e_side Binance::convert_position(order::Position p) const { 
        using namespace order;
        switch(p) {
            case Position::Buy: return binapi::e_side::buy;
            case Position::Sell: return binapi::e_side::sell;
        }
    }

    void Binance::prepare_tables() {
        using namespace std::string_literals;
        std::vector<std::string> lines =
            {"CREATE TABLE IF NOT EXISTS prices ("s
            ,"price_id SERIAL NOT NULL PRIMARY KEY,"
            ,"time BIGINT NOT NULL,"
            ,"symbol VARCHAR(32) NOT NULL,"
            ,"price DOUBLE PRECISION NOT NULL);"

            ,"CREATE TABLE IF NOT EXISTS orders ("
            ,"order_id SERIAL NOT NULL PRIMARY KEY,"
            ,"time BIGINT NOT NULL,"
            ,"order_type VARCHAR(255) NOT NULL,"
            ,"symbol VARCHAR(32) NOT NULL,"
            ,"position VARCHAR(8) NOT NULL,"
            ,"quantity DOUBLE PRECISION NOT NULL,"
            ,"price DOUBLE PRECISION NOT NULL)"
            ,"base_or_quote VARCHAR(8),"
            ,"time_in_force VARCHAR(8),"
            ,"limit DOUBLE PRECISION,"
            ,"stop DOUBLE PRECISION);"

            ,"CREATE TABLE IF NOT EXISTS account ("
            ,"account_id SERIAL NOT NULL PRIMARY KEY,"
            ,"time BIGINT NOT NULL,"
            ,"currency VARCHAR(8) NOT NULL,"
            ,"value DOUBLE PRECISION NOT NULL);"
            };

        auto create_query = common::utils::intercalate(" ",lines);
        m_database->execute(create_query);
    }

    void Binance::sync_price_history(time_t oldest) {
        using namespace std::string_literals;
        std::vector<std::string> lines =
            {"SELECT time,symbol,price"
            ,"FROM prices"
            ,"WHERE time >= $1;"
            };

        auto select_query = common::utils::intercalate(" ",lines);

        const auto prices = m_database->execute(select_query,oldest);

        m_price_history.clear();
        for(const auto &row : prices) {
            auto tpl = std::make_tuple(Symbol::from_string(row["symbol"].as<std::string>())
                                       ,row["price"].as<long double>());
            auto pair = std::make_pair(row["time"].as<time_t>(),tpl);
            m_price_history.insert(pair);
        }
    }

    void Binance::sync_order_history(time_t oldest) {
        using namespace std::string_literals;
        using namespace order;
        std::vector<std::string> lines =
            {"SELECT time,order_type,symbol,position,quantity,price,base_or_quote,time_in_force,limit,stop"
            ,"FROM orders"
            ,"WHERE time >= $1;"
            };

        auto select_query = common::utils::intercalate(" ",lines);

        const auto orders = m_database->execute(select_query,oldest);

        m_order_history.clear();
        for(const auto &row : orders) {
            auto price = row["price"].as<long double>();
            std::optional<BaseOrQuote> boq = bind(row["base_or_quote"].optional<std::string>()
                                                 ,boq_from_string);
            auto tif = bind(row["time_in_force"].optional<std::string>()
                           ,tif_from_string);
            auto limit = row["limit"].optional<long double>();
            auto stop = row["stop"].optional<long double>();

            GenericOrder order(otype_from_string(row["order_type"].as<std::string>()).value()
                              ,Symbol::from_string(row["symbol"].as<std::string>())
                              ,row["quantity"].as<long double>()
                              ,pos_from_string(row["position"].as<std::string>()).value()
                              );
            order.set_base_or_quote(boq);
            order.set_time_in_force(tif);
            order.m_limit = limit;
            order.m_stoptake = stop;

            order::Partial p{price,order};
            auto pair = std::make_pair(row["time"].as<time_t>(),p);
            m_order_history.insert(pair);
        }
    }

    void Binance::sync_account_history(time_t oldest) {
        using namespace std::string_literals;
        std::vector<std::string> lines =
            {"SELECT time,currency,value"
            ,"FROM account"
            ,"WHERE time >= $1;"
            };

        auto select_query = common::utils::intercalate(" ",lines);

        const auto funds = m_database->execute(select_query,oldest);

        m_account_history.clear();
        for(const auto &row : funds) {
            auto ccy = row["currency"].as<std::string>();
            auto val = row["value"].as<long double>();
            auto time = row["time"].as<time_t>();
    
            if(m_account_history.contains(time)) {
                m_account_history[time].add_fund(ccy,val);
            } else {
                account::Account acc;
                acc.add_fund(ccy,val);
                m_account_history.insert({time,acc});
            }
        }
    }


    Error<int> Binance::execute_order(order::GenericOrder go) {
        auto tif = 
            go.m_time_in_force.has_value() 
                ? convert_time_in_force(go.m_time_in_force.value()) 
                : binapi::e_time::FOK;

        auto resp = m_api.new_order(go.m_sym.to_string().c_str() //symbol
                                   ,convert_position(go.m_pos) // buy or sell
                                   ,convert_order_type(go.m_type) // market,limit,stoploss,etc..
                                   ,tif // TimeInForce: FOK,GTC,IOC
                                   ,binapi::e_trade_resp_type::FULL
                                   ,std::to_string(go.m_qty) // quantity
                                   ,nullptr // price, limit & stop loss limit
                                   ,"myid" // client id
                                   ,nullptr //stop price 
                                   ,nullptr //iceberg amount
                                   );

        if (!resp) {
            return resp.errmsg;
        }

        return 0;
    }

    Error<account::Account> Binance::get_account_info() {
        auto res = m_api.account_info();
        if(!res) {
            return res.errmsg;
        }

        std::map<std::string,long double> funds;
        for(const auto &kvp : res.v.balances) {
            funds.emplace(kvp.first,kvp.second.free);
        }
        return account::Account(funds);
    }

    Error<long double> Binance::get_account_value(time_t t) {
        auto acc = get_account_info();

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