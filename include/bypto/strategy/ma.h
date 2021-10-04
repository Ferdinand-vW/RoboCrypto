#pragma once

#include "bypto/data/kline.h"
#include "bypto/data/price.h"
#include "bypto/strategy.h"

#include <numeric>
#include <span>

namespace bypto::strategy {


    template<data::price::PriceSource P>
    class Strategy<Indicator::MovingAverage,P> {
        
        long double m_ma;
        std::span<data::price::Price<P>> m_interval;

        public:
            Strategy(std::span<data::price::Price<P>> &interval) : m_interval(interval) {};

            long double compute_moving_average() {
                auto sum = std::accumulate(m_interval.begin(),m_interval.end(),0,[](const int&acc,auto &kl) {
                    return acc + kl.m_close;
                });

                m_ma = sum / m_interval.size();

                return m_ma;
            }

            long double move_by_one(long double price) {

            }

            long double get_moving_average() {

            }
    };

}