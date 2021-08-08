// RoboCrypto.cpp : Defines the entry point for the application.
//

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

    return EXIT_SUCCESS;
}
