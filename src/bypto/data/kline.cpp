#include "bypto/data/kline.h"
#include "bypto/common/csv.h"
#include "bypto/common/types.h"
#include "bypto/common/utils.h"
#include "bypto/data/price.h"
#include "tao/pq/connection.hpp"

#include <algorithm>
#include <iomanip>
#include <iterator>
#include <ostream>
#include <sstream>
#include <string>

namespace bypto::data::price {

std::string Kline_t::to_string() const {
    std::stringstream ss;
    ss << "Kline {";
    ss << " m_open_time=" << std::put_time(localtime(&m_open_time), "%Y-%m-%d-%H-%M-%S");
    ss << ",m_open=" << m_open;
    ss << ",m_high=" << m_high;
    ss << ",m_low=" << m_low;
    ss << ",m_close=" << m_close;
    ss << ",m_volume=" << m_volume;
    ss << ",m_close_time=" << std::put_time(localtime(&m_close_time), "%Y-%m-%d-%H-%M-%S");
    ss << ",m_quote_asset_volume=" << m_quote_asset_volume;
    ss << ",m_number_of_trades=" << m_number_of_trades;
    ss << ",m_taker_buy_base_asset_volume=" << m_taker_buy_base_asset_volume;
    ss << ",m_taker_buy_quote_asset_volume=" << m_taker_buy_quote_asset_volume;
    ss << ",m_ignore }";

    return ss.str();
}

using namespace common::types;
using namespace common::utils;
Error<time_unit> infer_tick_rate(const std::vector<Kline_t> &klines) {
    if (klines.size() < 2) {
        return std::string("require at least 2 klines to infer tick rate");
    }

    auto it = klines.begin();
    auto kline1 = *it;
    it++;
    auto kline2 = *it;
    it++;
    auto tick_rate = diff_time(kline2.get_time(), kline1.get_time());
    auto prev_kline = kline2;
    for (; it != klines.end(); it++) {
        auto next_kline = *it;
        if (tick_rate != diff_time(next_kline.get_time(), prev_kline.get_time())) {
            return "Inferred tick rate " + to_string(tick_rate) + " does not match " + prev_kline.to_string() +
                   " and " + next_kline.to_string();
        }

        prev_kline = next_kline;
    }

    return tick_rate;
}

std::ostream &operator<<(std::ostream &os, const price::Kline_t &kl) {
    os << kl.to_string();
    return os;
}
} // namespace bypto::data::price