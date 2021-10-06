#pragma once

#include "bypto/common/types.h"
#include <bits/types/time_t.h>

namespace bypto::data::price {

    enum PriceSource { Kline, Spot };

    template<PriceSource P>
    class Price {

        public:
            long double get_price();
            time_t get_time();
            common::types::Symbol get_symbol();

    };

}