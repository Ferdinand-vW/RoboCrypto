#pragma once

#include "bypto/data/price.h"
#include "bypto/order.h"
#include "bypto/exchange.h"

namespace bypto::exchange {
    class Binance;

    template<>
    class Exchange<Binance,PriceSource::Spot> {
        int execute_order(order::GenericOrder go);
        long double fetch_price();
        void cancel_order(int o_id);
    };

}