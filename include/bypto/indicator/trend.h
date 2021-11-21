#pragma once

#include "bypto/data/price.h"
#include "bypto/data/prices.h"
#include "bypto/common/math.h"
#include "bypto/common/types.h"

#include <bits/types/time_t.h>
#include <numeric>
#include <span>

namespace bypto::indicator {
    using namespace data::prices;
    using namespace data::price;
    using namespace common::types;

    enum class TagIndicator { SimpleMA, ExponentialMA };

    template<PriceSource P>
    struct TrendParams {
        time_t m_begin;
        Prices<P> m_prices;
    };

    template <typename I>
    class TrendIndicator {
        public:
            template<PriceSource P>
            long double calculate(const TrendParams<P> &tp) {
                return static_cast<I*>(this)->calculate(tp);
            }
    };

    class SimpleMA : public TrendIndicator<SimpleMA> {
        public:
            static const struct SimpleMA_Tag : public Tag {} tag;

            template<PriceSource P>
            long double calculate(const TrendParams<P> &tp) {
                auto prices_in_period = tp.m_prices.most_recent(tp.m_begin);

                auto avg = common::math::average(prices_in_period,[](data::price::Price<P> &p) { return p.get_price(); });

                return avg;
            }
    };

    class ExponentialMA : public TrendIndicator<ExponentialMA> {
        //Closing price x multiplier + EMA (previous day) x (1-multiplier)

        long double m_previous = -1;
        long double m_smoothing = 2; //gives most recent observations more weights
                            //a higher number means more recent observations have
                            //more influence on EMA

        public:
            static const struct ExponentialMA_Tag : public Tag {} tag;

            template<PriceSource P>
            long double calculate(const TrendParams<P> &tp) {
                auto prices_in_period = tp.m_prices.most_recent(tp.m_begin);
                auto mprice = prices_in_period.front_opt();
                
                if(!mprice) { return 0; }
                if(m_previous < 0) { 
                    auto ema = mprice.value().get_price();
                    m_previous = ema;
                    return ema;
                }

                auto multiplier = m_smoothing / (1 + prices_in_period.size());
                auto ema = mprice.value().get_price() * multiplier + m_previous * (1 - multiplier);

                std::cout << "multiplier: " << multiplier << std::endl;
                std::cout << "m_smoothing: " << m_smoothing << std::endl;
                std::cout << "div: " << (prices_in_period.size()) << std::endl;
                std::cout << "ema: " << ema << std::endl;

                //update previous ema for next calculation
                m_previous = ema;

                return ema;
            }
    };
}