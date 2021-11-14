#pragma once

#include "bypto/data/price.h"
#include "bypto/data/prices.h"
#include "bypto/order/order.h"
#include "bypto/order/order_type.h"
#include "bypto/indicator/trend.h"

namespace bypto::strategy {

    using namespace data::price;

    template <PriceSource P,typename Ind
             ,template<PriceSource,typename> typename S
             ,template<PriceSource> typename C>
    class Strategy : S<P,Ind> {

        public:
            Strategy(C<P> &c) : S<P,Ind>(c){}; 

            common::types::Error<std::optional<order::Order<order::Market>>>
            make_decision(time_t now
                             ,long double spendable_qty
                             ,long double spendable_quote_qty
                             ,data::prices::Prices<P> prices) {
                return S<P,Ind>::make_decision(now,spendable_qty,spendable_quote_qty,prices);
            }

            bool has_enough_data(data::prices::Prices<P> prices) {
                return S<P,Ind>::has_enough_data(prices);
            }
    };

}