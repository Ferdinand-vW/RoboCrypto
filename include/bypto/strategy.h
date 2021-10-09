#pragma once

#include "bypto/data/price.h"
#include "bypto/data/prices.h"
#include "bypto/order.h"

namespace bypto::strategy {

    enum Indicator { MovingAverage };

    template <Indicator S,data::price::PriceSource P>
    class Strategy {

        public:
            common::types::Error<std::optional<order::Order>> 
                make_decision(time_t now
                             ,long double spendable_qty
                             ,long double spendable_quote_qty
                             ,data::prices::Prices<P> &prices);
    };

}