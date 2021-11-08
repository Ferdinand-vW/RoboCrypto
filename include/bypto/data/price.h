#pragma once

#include "bypto/common/types.h"
#include "bypto/common/utils.h"
#include <bits/types/time_t.h>

namespace bypto::data::price {

    enum class PriceSource { Kline, Spot };

    template<PriceSource P>
    class Price {

        public:
            long double get_price();
            time_t get_time();
            common::types::Symbol get_symbol();

            std::tuple<common::types::Symbol,time_t,long double> as_tuple();

    };

}