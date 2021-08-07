// RoboCrypto.cpp : Defines the entry point for the application.
//

#include "bypto/robo_crypto.h"
#include <binapi/api.hpp>

#include <boost/asio/io_context.hpp>

#include <iostream>

int main() {
    boost::asio::io_context ioctx;
    binapi::rest::api api{
         ioctx
        ,"api.binance.com"
        ,"443"
        ,"" // can be empty for non USER_DATA reqs
        ,"" // can be empty for non USER_DATA reqs
        ,10000 // recvWindow
    };

    auto res = api.price("BTCUSDT");
    if (!res) {
        std::cerr << "get price error: " << res.errmsg << std::endl;

        return EXIT_FAILURE;
    }

    std::cout << "price: " << res.v << std::endl;

    return EXIT_SUCCESS;
}
