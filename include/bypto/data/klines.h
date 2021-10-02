#pragma once

#include "bypto/common/types.h"

#include "tao/pq/connection.hpp"

#include <bits/types/time_t.h>
#include <ctime>
#include <istream>
#include <vector>
#include <deque>
#include <span>

namespace bypto::data::klines {

    // Candle data
    struct Kline {
        time_t m_open_time; //start kline time
        long double m_open; // price at @m_open_time
        long double m_high; // highest price between @m_open_time and @m_close_time
        long double m_low; // lowest price between @m_open_time and @m_close_time
        long double m_close; //price at @m_close_time
        long double m_volume; // sum of traded quantity in base asset
        time_t m_close_time; //end kline time
        long double m_quote_asset_volume; // sum of traded quantity in non-base asset
        long m_number_of_trades;
        long double m_taker_buy_base_asset_volume; // How much of @volume was taker buy orders
        long double m_taker_buy_quote_asset_volume; // How much of @quote_asset_volume was taker buy orders
        long double m_ignore;
    };

    class KlineData {
        std::vector<Kline> m_klines;

        public:
            KlineData(std::vector<Kline> &&klines) : m_klines(std::move(klines)) {}

            std::span<Kline> time_interval(time_t start, time_t end) {
                return m_klines;
            }
            std::span<Kline> most_recent(time_t time_period) {
                return m_klines;
            }


            Kline& operator[](int index) {
                return m_klines[index];
            }

            std::vector<Kline>& get_data() {
                return m_klines;
            }

            std::optional<Kline> front_opt() {
                if(m_klines.size() == 0) {
                    return {};
                } else {
                    return m_klines.front();
                }
            }

            std::optional<Kline> index_opt(int index) {
                if(m_klines.size() > index) { return m_klines[index]; }
                else { return {};}
            }

            size_t size() {
                return m_klines.size();
            }

            auto begin() {
                return m_klines.begin();
            }
            auto end() {
                return m_klines.end();
            }



    };

    std::ostream& operator<<(std::ostream&os,const Kline &kl);
}