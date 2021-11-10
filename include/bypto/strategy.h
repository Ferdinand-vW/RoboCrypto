#pragma once

#include "bypto/data/price.h"
#include "bypto/data/prices.h"
#include "bypto/order/order.h"
#include "bypto/order/order_type.h"

namespace bypto::strategy {

    using namespace data::price;

    template <template<PriceSource> typename S,PriceSource P>
    class Strategy : S<P> {

        public:
            common::types::Error<std::optional<order::Order<order::Market>>>
            make_decision(time_t now
                             ,long double spendable_qty
                             ,long double spendable_quote_qty
                             ,data::prices::Prices<P> prices) {
                return S<P>::make_decision(now,spendable_qty,spendable_quote_qty,prices);
            }

            bool has_enough_data(data::prices::Prices<P> prices) {
                return S<P>::has_enough_data(prices);
            }
    };

}