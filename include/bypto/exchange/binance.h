#pragma once

#include "bypto/order.h"
#include "bypto/exchange.h"

namespace bypto::exchange {
    class Binance;

    template<>
    class Exchange<Binance> {
        int execute_order(order::GenericOrder go);
        long double fetch_price();
        void cancel_order(int o_id);
    };

}