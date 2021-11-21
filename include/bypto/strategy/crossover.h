#pragma once

#include "bypto/common/types.h"
#include "bypto/common/either.h"
#include "bypto/data/kline.h"
#include "bypto/data/spot.h"
#include "bypto/data/price.h"
#include "bypto/data/prices.h"
#include "bypto/order/order.h"
#include "bypto/order/order_type.h"
#include "bypto/strategy.h"
#include "bypto/indicator/trend.h"

#include <numeric>
#include <optional>
#include <span>
#include <string>

namespace bypto::strategy {

    using namespace common::types;
    using namespace data::prices;
    using namespace order;
    using namespace indicator;

    template<data::price::PriceSource P>
    class CollectCrossover {
        std::vector<time_t> m_times;
        std::vector<data::prices::Price<P>> m_prices;
        std::vector<long double> m_fast;
        std::vector<long double> m_slow;
        std::set<time_t> m_crossovers;
        public:
            //here sma=short moving average and lma = long moving average
            //short/long indicates the period and which is faster/slower moving
            void put(time_t t, data::prices::Price<P> p, long double fast_ma, long double slow_ma) {
                m_times.push_back(t);
                m_prices.push_back(p);
                m_fast.push_back(fast_ma);
                m_slow.push_back(slow_ma);
            }

            void put_crossover(time_t time) {
                m_crossovers.insert(time);
            }

            auto get() {
                std::vector<std::tuple<time_t,data::prices::Price<P>,long double, long double>> res;
                for(int i = 0; i < m_times.size(); i++) {
                    res.push_back(std::make_tuple(m_times[i],m_prices[i],m_fast[i],m_slow[i]));
                }
                return res;
            }

            auto csv_data() {
                std::vector<std::tuple<std::string,long double,long double, long double,std::string>> res;
                for(int i = 0; i < m_times.size(); i++) {
                    auto pp_co = m_crossovers.contains(m_times[i]) 
                                    ? common::utils::pp_time(m_times[i]) 
                                    : "";
                    res.push_back(std::make_tuple(common::utils::pp_time(m_times[i])
                                                 ,m_prices[i].get_price()
                                                 ,m_fast[i],m_slow[i]
                                                 ,pp_co));
                }
                return res;
            }

            std::array<std::string,5> csv_header() {
                return {"TIME","PRICE","SLOW","FAST","CROSS OVER"};
            }
    };

    // 4 Moving Averages shortold short new long old long new by time and price
    template<typename Ind,data::price::PriceSource P>
    class Crossover : public Strategy<Crossover<Ind,P>,P> {

        private:
            CollectCrossover<P> m_collector;
            TrendIndicator<Ind> m_indicator;

            long double m_slow = -1;
            long double m_fast = -1;

        public:
            static const struct Crossover_Tag : public Tag {} tag;

            Crossover(TrendIndicator<Ind> &&indicator) : m_indicator(std::move(indicator)),m_collector(CollectCrossover<P>()) {};

            Error<std::optional<Order<Market>>> 
            make_decision(time_t now
                         ,long double spendable_qty
                         ,long double spendable_quote_qty
                         ,const Prices<P> & prices) {
                
                if (!has_enough_data(prices)) {
                    using namespace std::literals::string_literals;
                    return "Too few data points to apply Crossover strategy. Require at least 16 points which is 16xtime_interval worth of time."s;
                }

                //naming of variables is for the example. Actual times will depend on the time interval of @prices
                time_t slow_moving_time = prices[prices.size() - 16].get_time();
                time_t fast_moving_time = prices[prices.size() - 4].get_time();

                auto slow_tp = TrendParams<P> { slow_moving_time, prices};
                auto slow_moving = m_indicator.calculate(slow_tp);

                auto fast_tp = TrendParams<P> { fast_moving_time, prices};
                auto fast_moving = m_indicator.calculate(fast_tp);

                if(m_slow == -1) { m_slow = slow_moving; }
                if(m_fast == -1) { m_fast = fast_moving; }

                common::types::Symbol sym("BTC","USDT");
                //short ma moves above long ma indicating rising trend
                //which means we should buy base ccy
                std::optional<Order<Market>> res;
                if(fast_moving > slow_moving && m_fast < m_slow) {
                    //buy base ccy, pay quote ccy
                    Market mkt{BaseOrQuote::Quote};
                    Order ord(sym,spendable_quote_qty,Position::Sell,mkt);
                    res = ord;
                    m_collector.put_crossover(now);
                    //short ma moves below long ma indicating falling trend
                    //which means we should sell base ccy
                } else if (fast_moving < slow_moving && m_fast > m_slow) {
                    //sell base ccy, receive quote ccy
                    Market mkt{BaseOrQuote::Base};
                    order::Order ord(sym,spendable_qty,Position::Sell,mkt);
                    res = ord;
                    m_collector.put_crossover(now);
                }

                auto price_now = prices.back_opt();
                if(price_now) {  //write to collector if price is present
                    m_collector.put(now,price_now.value(),fast_moving, slow_moving);
                }
                //save previously computed moving averages so that we can compare against these in the next iteration
                m_slow = slow_moving;
                m_fast = fast_moving;

                return res;
            }

            bool has_enough_data(const Prices<P>& prices) {
                // E.g. 15 min interval then 4 hour MA against 1 hour MA
                // 4h/15min = 16. We probably want this to be configurable at call site.
                return prices.size() >= 16;
            }
    };

}