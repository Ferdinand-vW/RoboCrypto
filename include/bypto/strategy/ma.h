#pragma once

#include "bypto/common/types.h"
#include "bypto/common/either.h"
#include "bypto/data/kline.h"
#include "bypto/data/price.h"
#include "bypto/data/prices.h"
#include "bypto/order.h"
#include "bypto/strategy.h"

#include <numeric>
#include <span>
#include <string>

namespace bypto::strategy {


    template<data::price::PriceSource P>
    class Strategy<Indicator::MovingAverage,P> {

        private:
            long double m_ma;

            common::types::Error<long double> compute_moving_average(time_t oldest,data::prices::Prices<P> &prices) {
                auto intv = prices.m_interval;

                auto prices_in_period = prices.most_recent(oldest);
                auto sum = std::accumulate(prices_in_period.begin(),prices_in_period.end(),0,[](const int&acc,auto &kl) {
                    return acc + kl.m_close;
                });

                m_ma = sum / prices_in_period.size();

                return m_ma;
            }

        public:
            common::types::Error<std::optional<order::Order>> make_decision(time_t now
                                                      ,long double spendable_qty
                                                      ,long double spendable_quote_qty
                                                      ,data::prices::Prices<P> &prices) {
                //E.g. 15 min interval then 4 hour MA against 1 hour MA
                //4h/15min = 16. We probably want this to be configurable at call site.
                if (prices.size() < 16) {
                    return std::string("Too few data points to apply MA strategy. Require at least 16 points which is 16xtime_interval worth of time.");
                }

                //naming of variables is for the example. Actual times will depend on the time interval of @prices
                time_t four_hour = prices[prices.size() - 16];
                time_t one_hour = prices[prices.size() - 5];

                auto four_hour_ma = compute_moving_average(four_hour, prices);
                auto one_hour_ma = compute_moving_average(one_hour, prices);

                if(one_hour_ma > four_hour_ma) {
                    //buy base ccy, use quote ccy
                } else if (one_hour_ma < four_hour_ma) {
                    //sell base ccy, receive quote ccy
                } else {
                    std::optional<order::Order> nothing = {};
                    return nothing;
                }

            }
    };

}