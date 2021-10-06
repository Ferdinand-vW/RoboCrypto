// RoboCrypto.cpp : Defines the entry point for the application.
//


#include "bypto/common/utils.h"
#include "bypto/data/binance.h"
#include "bypto/data/price.h"
#include "bypto/common/csv.h"
#include "bypto/order.h"
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


    fs.open("/home/ferdinand/dev/bypto/historical/binance/kline/BTCUSDT-15m-2021-07.csv");
    std::cout << fs.is_open() << std::endl;
    using str_t = std::string;

    auto klines = bypto::data::binance::parseCSV("BTCUSDT",fs);
    fs.close();
    for(auto i = 0; i < 2; i++) {
        std::cout << " " << klines[i] << std::endl;
    }

    auto conn = tao::pq::connection::create("dbname=historical");
    
    using namespace bypto::data;
    binance::prepareTable(conn);
    binance::storeKlines(conn,klines);

    using namespace bypto::common;
    auto t = utils::createTime_t(2021, 07, 01);
    auto open_time = utils::createTime_t(2021,07,06);
    auto close_time = utils::createTime_t(2021,07,20);

    auto klines2 = binance::loadKlines(conn, open_time, close_time);
    std::cout << klines2.front_opt().value() << std::endl;
    using namespace bypto::order;
    using namespace bypto::order_type;
    Order order {"BTCUSDT",Position::Buy,Market { 1.0,Base }};

    std::cout << order;
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
