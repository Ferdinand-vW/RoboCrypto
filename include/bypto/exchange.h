#pragma once

#include "bypto/data/binance/klines.h"
#include "bypto/order.h"

namespace bypto::exchange {
    using namespace data::binance::klines;

    template<typename T>
    class Exchange {
        public:
            virtual int execute_order(order::Order order);
            virtual long double fetch_price();
            virtual void cancel_order(int o_id);
            virtual bool tick_once();
            virtual std::vector<Kline> historical_data(time_t start,time_t end);
    };
}