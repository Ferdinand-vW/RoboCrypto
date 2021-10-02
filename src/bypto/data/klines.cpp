#include "bypto/data/klines.h"
#include "bypto/common/csv.h"
#include "bypto/common/types.h"
#include "bypto/common/utils.h"
#include "tao/pq/connection.hpp"

#include <algorithm>
#include <iomanip>
#include <iterator>
#include <ostream>

namespace bypto::data::klines {

    
    std::ostream& operator<<(std::ostream &os,const Kline &kl) {
        os << "Kline {";
        os << " m_open_time=" << std::put_time(localtime(&kl.m_open_time),"%Y-%m-%d-%H-%M-%S");
        os << ",m_open=" << kl.m_open;
        os << ",m_high=" << kl.m_high;
        os << ",m_low=" << kl.m_low;
        os << ",m_close=" << kl.m_close;
        os << ",m_volume=" << kl.m_volume;
        os << ",m_close_time=" << std::put_time(localtime(&kl.m_close_time),"%Y-%m-%d-%H-%M-%S");
        os << ",m_quote_asset_volume=" << kl.m_quote_asset_volume;
        os << ",m_number_of_trades=" << kl.m_number_of_trades;
        os << ",m_taker_buy_base_asset_volume=" << kl.m_taker_buy_base_asset_volume;
        os << ",m_taker_buy_quote_asset_volume=" << kl.m_taker_buy_quote_asset_volume;
        os << ",m_ignore }";
        return os;
    }
}