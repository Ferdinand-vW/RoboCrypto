#pragma once

#include "bypto/order.h"
#include "bypto/exchange.h"

namespace bypto::exchange::binance {

    class Binance : public Exchange {
        int execute_order(order::GenericOrder go);
        long double fetch_price();
        void cancel_order(int o_id);
    };

}