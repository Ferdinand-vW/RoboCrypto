#pragma once

#include <vector>
#include <span>
#include <optional>

#include "bypto/common/utils.h"
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
                using namespace common::utils;
                auto start_pred = [start](auto &p) { return compare_time(p.get_time(),start);};
                auto end_pred = [end](auto &p) { return compare_time(p.get_time(),end);};
                auto start_i = bin_search_index(m_prices, start_pred);
                auto end_i = bin_search_index(m_prices, end_pred);

                //We have to assume that either or both start and end are time_t values that we cannot find
                //in m_prices. Should either one be missing then let's assume they were not provided
                if(start_i >= 0 && end_i >= 0 && start_i <= end_i) {
                    return Prices(m_interval,m_prices.subspan(start_i,end_i - start_i + 1));
                } else if(start_i >= 0) {
                    return Prices(m_interval,m_prices.subspan(start_i));
                } else if(end_i >= 0) {
                    return Prices(m_interval,m_prices.subspan(0,end_i + 1));
                }

                return Prices(m_interval,m_prices);
            }

            //Can be implemented using @time_interval@, however this is slightly faster
            //since we only do a binary search once instead of twice
            Prices<P> most_recent(time_t time_period) const {
                using namespace common::utils;
                
                auto pred = [time_period](auto &p) { return compare_time(p.get_time(), time_period); };

                //Find the index of @time_period@ in price span
                auto i = common::utils::bin_search_index(m_prices,pred);
                if(i >= 0) {
                    return Prices(m_interval,m_prices.subspan(i));
                }

                //if we cannot find @time_period@ then simply return original span
                return Prices(m_interval,m_prices);
            }


            Price<P> operator[](int index) const {
                return m_prices[index];
            }

            std::span<Price<P>>& get_data() {
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