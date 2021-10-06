#pragma once

#include "bypto/data/kline.h"
#include "bypto/data/price.h"
#include "bypto/data/prices.h"
#include "bypto/strategy.h"

#include <numeric>
#include <span>
#include <string>

namespace bypto::strategy {


    template<data::price::PriceSource P>
    class Strategy<Indicator::MovingAverage,P> {

        private:
            long double m_ma;

            common::types::Error<long double> compute_moving_average(time_t now,data::prices::Prices<P> &prices) {
                auto intv = prices.m_interval;
                //E.g. 15 min interval then 4 hour MA against 1 hour MA
                //4h/15min = 16. We probably want this to be configurable at call site.
                if (prices.size() < 16) {
                    return std::string("Too few data points to apply MA strategy. Require at least 16 points which is 16xtime_interval worth of time.");
                }

                auto latest_price = prices[prices.size() - 1]; //last price
                auto base_price = prices[prices.size() - 16]; //16th price
                auto comp_price = prices[prices.size() - 5]; //base price for comparison
                
                prices.time_interval(time_t start, time_t end)
                auto sum = std::accumulate(interval.begin(),interval.end(),0,[](const int&acc,auto &kl) {
                    return acc + kl.m_close;
                });

                m_ma = sum / interval.size();

                return m_ma;
            }

        public:
            Error<
    };

}