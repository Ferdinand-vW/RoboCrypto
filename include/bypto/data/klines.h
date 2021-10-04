#pragma once

#include "bypto/common/types.h"
#include "bypto/data/kline.h"

#include "tao/pq/connection.hpp"

#include <bits/types/time_t.h>
#include <ctime>
#include <istream>
#include <vector>
#include <deque>
#include <span>

namespace bypto::data::klines {


    class KlineData {
        std::vector<price::Kline_t> m_klines;

        public:
            KlineData(std::vector<price::Kline_t> &&klines) : m_klines(std::move(klines)) {}

            std::span<price::Kline_t> time_interval(time_t start, time_t end) {
                return m_klines;
            }
            std::span<price::Kline_t> most_recent(time_t time_period) {
                return m_klines;
            }


            price::Kline_t& operator[](int index) {
                return m_klines[index];
            }

            std::vector<price::Kline_t>& get_data() {
                return m_klines;
            }

            std::optional<price::Kline_t> front_opt() {
                if(m_klines.size() == 0) {
                    return {};
                } else {
                    return m_klines.front();
                }
            }

            std::optional<price::Kline_t> index_opt(int index) {
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
}