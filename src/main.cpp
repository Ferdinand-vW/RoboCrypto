// RoboCrypto.cpp : Defines the entry point for the application.
//

#include "bypto/common/utils.h"
#include "bypto/data/binance.h"
#include "bypto/data/price.h"
#include "bypto/common/csv.h"
#include "bypto/exchange/back_testing.h"
#include "bypto/order/order.h"
#include "bypto/common/either.h"
#include "bypto/strategy.h"
#include "bypto/exchange/runner.h"

#include <boost/asio/io_context.hpp>

#include <binapi/api.hpp>
#include <binapi/enums.hpp>
#include <tao/pq.hpp>
#include <tao/pq/connection.hpp>

#include <cstdlib>
#include <fstream>
#include <iostream>
#include <string>

using namespace bypto::common::either;

int main() {
    std::fstream fs;


    //parse binance klines historical data
    fs.open("/home/ferdinand/dev/bypto/historical/binance/kline/BTCUSDT-15m-2021-07.csv");
    bypto::common::types::Symbol sym("BTC","USDT");
    auto klines = bypto::data::binance::parseCSV(sym,fs);
    fs.close();

    auto conn = tao::pq::connection::create("dbname=historical");
    
    //store historical data in database
    using namespace bypto::data;
    binance::prepareTable(conn);
    binance::storeKlines(conn,klines);

    using namespace bypto::common;
    auto t = utils::create_time(2021, 07, 01);
    auto open_time = utils::create_time(2021,07,06);
    auto close_time = utils::create_time(2021,07,20);

    using namespace bypto::exchange;
    auto start_time = klines.front().m_close_time;
    std::cout << "start time " << start_time << std::endl;
    utils::time_unit fifteen_minutes = {0,0,0,0,15,0};

    std::cout << fifteen_minutes << std::endl;
    std::cout << utils::add_time(start_time,fifteen_minutes) << std::endl;
    BackTestExchange bte(sym,1000,1000,start_time,fifteen_minutes,std::move(klines));
    runner::BackTestRunner bt_runner(bte);

    using namespace bypto::strategy;
    Strategy<MovingAverage,PriceSource::Kline> strat_ma;

    auto res = bt_runner.run(sym, strat_ma);
    std::cout << res << std::endl;

    auto ev = bte.get_account_value();

    std::cout << ev.right() << std::endl;

    // auto open_time = t
    // const auto pk = std::getenv("BINANCE_TEST_PUBLIC_KEY");
    // const auto sk = std::getenv("BINANCE_TEST_SECRET_KEY");

    // std::cout << "public key: " << pk << std::endl;
    // std::cout << "secret key: " << sk << std::endl;
    // boost::asio::io_context io;
    // binapi::rest::api api{
    //      io
    //     ,"testnet.binance.vision"
    //     ,"443"
    //     ,pk // can be empty for non USER_DATA reqs
    //     ,sk // can be empty for non USER_DATA reqs
    //     ,10000 // recvWindow
    // };

    // auto res = api.price("BTCUSDT");
    // if (!res) {
    //     std::cerr << "get price error: " << res.errmsg << std::endl;

    //     return EXIT_FAILURE;
    // }

    // std::cout << "price: " << res.v << std::endl;

    // auto account = api.account_info();
    // if ( !account ) {
    //     std::cerr << "account info error: " << account.errmsg << std::endl;
    //     return EXIT_FAILURE;
    // }

    // std::cout << "account info: " << account.v << std::endl << std::endl;

    // // auto resp = api.new_order("BTCUSDT",binapi::e_side::buy,binapi::e_type::market,binapi::e_time::FOK,binapi::e_trade_resp_type::FULL
    // //                          ,"0.001",nullptr,"myid",nullptr,nullptr);

    // // std::cout << "new order err " << resp.errmsg << std::endl;
    // // std::cout << "new order " << resp.reply << std::endl;

    // auto orders = api.all_orders("BTCUSDT");
    // std::cout << "order error" << orders.errmsg << std::endl;
    // std::cout << "orders " << orders.reply << std::endl;
    // for (auto &o : orders.v.orders) {
    //     std::cout << o.first << std::endl;
    //     for(auto &oi : o.second) {
    //         std::cout << oi.clientOrderId << " " << oi.orderId << " " << oi.cummulativeQuoteQty << " " << oi.executedQty;
    //         std::cout << " " << oi.icebergQty << " " << oi.isWorking << " " << oi.origQty << " " << oi.price << " ";
    //         std::cout << oi.stopPrice << " " << oi.side << " " << oi.status << " " << oi.symbol << " " << oi.time << " ";
    //         std::cout << oi.timeInForce << " " << oi.updateTime << " " << oi.type << std::endl;
    //     }
    // }
    
    return EXIT_SUCCESS;
}
