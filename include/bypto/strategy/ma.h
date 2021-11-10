#pragma once

#include "bypto/common/types.h"
#include "bypto/common/either.h"
#include "bypto/data/kline.h"
#include "bypto/data/price.h"
#include "bypto/data/prices.h"
#include "bypto/order/order.h"
#include "bypto/order/order_type.h"
#include "bypto/strategy.h"

#include <numeric>
#include <optional>
#include <span>
#include <string>

namespace bypto::strategy {

    using namespace common::types;
    using namespace order;
    template<data::price::PriceSource P>
    class MovingAverage {

        private:
            long double m_long_ma = -1;
            long double m_short_ma = -1;

            long double compute_moving_average(time_t oldest,const Klines_t& prices) {
                if(prices.size() <= 0) { return 0; } // prevent divide by 0

                const std::span<const Kline_t> prices_in_period = prices.most_recent(oldest);
                auto sum = std::accumulate(prices_in_period.begin(),prices_in_period.end(),0,[](const int&acc,auto &kl) {
                    return acc + kl.m_close;
                });

                return sum / prices_in_period.size();
            }

        public:
            Error<std::optional<Order<Market>>> 
            make_decision(time_t now
                         ,long double spendable_qty
                         ,long double spendable_quote_qty
                         ,const Klines_t& prices) {
                
                if (!has_enough_data(prices)) {
                    using namespace std::literals::string_literals;
                    return "Too few data points to apply MA strategy. Require at least 16 points which is 16xtime_interval worth of time."s;
                }

                //naming of variables is for the example. Actual times will depend on the time interval of @prices
                time_t four_hour = prices[prices.size() - 16].get_time();
                time_t one_hour = prices[prices.size() - 5].get_time();

                auto four_hour_ma = compute_moving_average(four_hour, prices);
                auto one_hour_ma = compute_moving_average(one_hour, prices);

                if(m_long_ma == -1) { m_long_ma = four_hour_ma; }
                if(m_short_ma == -1) { m_short_ma = one_hour_ma; }

                common::types::Symbol sym("BTC","USDT");
                //short ma moves above long ma indicating rising trend
                //which means we should buy base ccy
                std::optional<Order<Market>> res;
                if(one_hour_ma > four_hour_ma && one_hour_ma < m_long_ma) {
                    //buy base ccy, pay quote ccy
                    Market mkt{BaseOrQuote::Quote};
                    Order ord(sym,spendable_quote_qty,Position::Sell,mkt);
                    res = ord;
                    std::cout << "ohm: " << one_hour_ma;
                    std::cout << ",fhm: " << four_hour_ma;
                    std::cout << ",lma: " << m_long_ma;
                    std::cout << ": Short crosses over Long MA" << std::endl;
                    //short ma moves below long ma indicating falling trend
                    //which means we should sell base ccy
                } else if (one_hour_ma < four_hour_ma && one_hour_ma > m_long_ma) {
                    //sell base ccy, receive quote ccy
                    Market mkt{BaseOrQuote::Base};
                    order::Order ord(sym,spendable_qty,Position::Sell,mkt);
                    res = ord;
                    std::cout << "ohm: " << one_hour_ma;
                    std::cout << ",fhm: " << four_hour_ma;
                    std::cout << ",lma: " << m_long_ma;
                    std::cout << ": Long crosses over Short MA" << std::endl;
                }

                //save previously computed moving averages so that we can compare against these in the next iteration
                m_long_ma = four_hour_ma;
                m_short_ma = one_hour_ma;

                return res;
            }

            bool has_enough_data(const Klines_t& prices) {
                // E.g. 15 min interval then 4 hour MA against 1 hour MA
                // 4h/15min = 16. We probably want this to be configurable at call site.
                return prices.size() >= 16;
            }
    };

}