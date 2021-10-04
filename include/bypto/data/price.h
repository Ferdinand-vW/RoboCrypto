#pragma once

namespace bypto::data::price {

    enum PriceSource { Kline, Spot };

    template<PriceSource P>
    class Price {

        public:
            long double get_price();

    };

}