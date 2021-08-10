// RoboCrypto.cpp : Defines the entry point for the application.
//

#include "binapi/enums.hpp"
#include "bypto/robo_crypto.h"

#include <boost/asio/io_context.hpp>
#include <binapi/api.hpp>

#include <cstdlib>
#include <iostream>

int main() {
    const auto pk = std::getenv("BINANCE_TEST_PUBLIC_KEY");
    const auto sk = std::getenv("BINANCE_TEST_SECRET_KEY");

    std::cout << "public key: " << pk << std::endl;
    std::cout << "secret key: " << sk << std::endl;
    boost::asio::io_context io;
    binapi::rest::api api{
         io
        ,"testnet.binance.vision"
        ,"443"
        ,pk // can be empty for non USER_DATA reqs
        ,sk // can be empty for non USER_DATA reqs
        ,10000 // recvWindow
    };

    auto res = api.price("BTCUSDT");
    if (!res) {
        std::cerr << "get price error: " << res.errmsg << std::endl;

        return EXIT_FAILURE;
    }

    std::cout << "price: " << res.v << std::endl;

    auto account = api.account_info();
    if ( !account ) {
        std::cerr << "account info error: " << account.errmsg << std::endl;
        return EXIT_FAILURE;
    }

    std::cout << "account info: " << account.v << std::endl << std::endl;

    auto resp = api.new_order("BTCUSDT",binapi::e_side::buy,binapi::e_type::market,binapi::e_time::FOK,binapi::e_trade_resp_type::FULL
                             ,"20",nullptr,"myid",nullptr,nullptr);

    std::cout << "new order err " << resp.errmsg << std::endl;
    std::cout << "new order " << resp.reply << std::endl;

    auto orders = api.all_orders("BTCUSDT");
    std::cout << "order error" << orders.errmsg << std::endl;
    std::cout << "orders " << orders.reply << std::endl;
    for (auto &o : orders.v.orders) {
        std::cout << o.first << std::endl;
        for(auto &oi : o.second) {
            std::cout << oi.clientOrderId << " " << oi.orderId << " " << oi.cummulativeQuoteQty << " " << oi.executedQty;
            std::cout << " " << oi.icebergQty << " " << oi.isWorking << " " << oi.origQty << " " << oi.price << " ";
            std::cout << oi.stopPrice << " " << oi.side << " " << oi.status << " " << oi.symbol << " " << oi.time << " ";
            std::cout << oi.timeInForce << " " << oi.updateTime << " " << oi.type << std::endl;
        }
    }
    

    return EXIT_SUCCESS;
}
