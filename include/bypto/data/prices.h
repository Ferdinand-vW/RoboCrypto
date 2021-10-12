#pragma once

#include <vector>
#include <span>
#include <optional>

#include "bypto/data/price.h"
#include "bypto/common/types.h"

namespace bypto::data::prices {
    using namespace bypto::data::price;

    template<PriceSource P>
    class Prices {

        private:
            std::span<Price<P>> m_prices;

            Prices(time_t interval,std::vector<Price<P>> &prices) : m_interval(interval)
                                                                  , m_prices(prices) {}

            Prices(time_t interval,std::span<Price<P>> prices) : m_interval(interval)
                                                               , m_prices(prices) {}

        public:
            time_t m_interval;



            //Overrides any existing data. Performs sanity check.
            static common::types::Error<Prices<P>> CreatePrices(time_t interval,std::vector<Price<P>> &prices) {
                if(prices.size() <= 0) {
                    return Prices(interval,prices);
                }

                //confirm that every price was taken at an interval of @interval@
                time_t t0 = prices.front().get_time();
                //i = 1 since we start with time of index 0
                for(int i = 1;i<prices.size();i++) {
                    if(t0+interval != prices[i].get_time()) {
                        return "Non matching time interval for prices: " + std::to_string(t0) + " and " + std::to_string(prices[i].get_time());
                    }

                    t0 = prices[i].get_time();
                }

                return Prices(interval,prices);
            }
            
            //This price should be exactly taken at @m_interval@ time after the last price in @m_prices@
            //We may apply interpolation here..
            bool addNewPrice(Price<P> p) {
                if(m_prices.size() <= 0 
                || m_prices.back().get_time() + m_interval == p.get_time()) {
                    m_prices.push_back(p);
                    return true;
                } else {
                    return false;
                }
            }

            Prices<P> time_interval(time_t start, time_t end) const {
                return m_prices;
            }

            Prices<P> most_recent(time_t time_period) const {
                return Prices(m_interval,m_prices);
            }


            Price<P> operator[](int index) const {
                return m_prices[index];
            }

            std::vector<Price<P>>& get_data() {
                return m_prices;
            }

            std::optional<Price<P>> front_opt() const {
                if(m_prices.size() == 0) {
                    return {};
                } else {
                    return m_prices.front();
                }
            }

            std::optional<Price<P>> back_opt() const {
                if(m_prices.size() == 0) {
                    return {};
                } else {
                    return m_prices.back();
                }
            }

            std::optional<Price<P>> index_opt(int index) const {
                if(m_prices.size() > index) { return m_prices[index]; }
                else { return {};}
            }

            size_t size() const {
                return m_prices.size();
            }

            auto begin() const {
                return m_prices.begin();
            }
            auto end() const {
                return m_prices.end();
            }
    };
}