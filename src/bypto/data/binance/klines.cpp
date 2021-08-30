#include "bypto/data/binance/klines.h"
#include "bypto/common/csv.h"
#include <algorithm>
#include <iomanip>
#include <iterator>
#include <ostream>

namespace bypto::data::binance::klines {

    std::vector<Kline> parseCSV(std::istream &is) {
        typedef long double ld;
        //sadly type inference is unable to figure out what
        //the type of the header should be
        auto tpls = common::csv::parse
                        <time_t,ld,ld,ld,ld,ld
                        ,time_t,ld,long,ld,ld,ld>(is);

        auto toKline = [](auto &tpl) {
            return Kline
                { std::get<0>(tpl)/1000 // binance timestamps are in milliseconds, time_t is in seconds
                , std::get<1>(tpl)
                , std::get<2>(tpl)
                , std::get<3>(tpl)
                , std::get<4>(tpl)
                , std::get<5>(tpl)
                , std::get<6>(tpl)/1000 // binance timestamps are in milliseconds, time_t is in seconds
                , std::get<7>(tpl)
                , std::get<8>(tpl)
                , std::get<9>(tpl)
                , std::get<10>(tpl)
                , std::get<11>(tpl)
                };
        };

        std::vector<Kline> klines;
        std::transform(tpls.begin(),tpls.end(),std::back_inserter(klines),toKline);

        return klines;
        
    }

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