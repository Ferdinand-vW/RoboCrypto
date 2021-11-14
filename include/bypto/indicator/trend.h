#pragma once

#include "bypto/data/price.h"
#include "bypto/data/prices.h"
#include "bypto/common/math.h"

#include <bits/types/time_t.h>
#include <numeric>
#include <span>

namespace bypto::indicator {
    using namespace data::prices;
    using namespace data::price;

    template <typename I>
    class TrendIndicator : I {
        public:
            template<PriceSource P>
            static long double calculate(time_t oldest,const Prices<P>& prices) {
                return I::calculate(oldest,prices);
            }
    };

    class MovingAverage {
        public:
            template<PriceSource P>
            static long double calculate(time_t oldest,const Prices<P>& prices) {
                auto prices_in_period = prices.most_recent(oldest);

                auto avg = common::math::average(prices_in_period,[](Price<P> &p) { return p.get_price(); });

                return avg;
            }
    };
}