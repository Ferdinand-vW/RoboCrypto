#pragma once

#include "bypto/data/price.h"
#include "bypto/order.h"

#include <span>

namespace bypto::exchange {
    using namespace data::price;

    template<typename T,PriceSource P>
    class Exchange {
        public:
            virtual int execute_order(order::Order order);
            virtual long double fetch_price();
            virtual void cancel_order(int o_id);
            virtual bool tick_once();
            virtual std::span<Price<P>> historical_data(time_t start,time_t end);
    };
}