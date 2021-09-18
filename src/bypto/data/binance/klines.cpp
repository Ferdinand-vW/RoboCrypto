#include "bypto/data/binance/klines.h"
#include "bypto/common/csv.h"
#include "bypto/common/types.h"
#include "bypto/common/utils.h"
#include "tao/pq/connection.hpp"

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

    void prepareTable(common::types::pgconn_t &conn) {
        using namespace std::string_literals;
        std::vector<std::string> parts = 
            {"CREATE TABLE IF NOT EXISTS klines ("s
            ,"kline_id SERIAL NOT NULL PRIMARY KEY,"
            ,"open_time BIGINT NOT NULL,"
            ,"open DOUBLE PRECISION NOT NULL,"
            ,"high DOUBLE PRECISION NOT NULL,"
            ,"low DOUBLE PRECISION NOT NULL,"
            ,"close DOUBLE PRECISION NOT NULL,"
            ,"volume DOUBLE PRECISION NOT NULL,"
            ,"close_time BIGINT NOT NULL,"
            ,"quote_asset_volume DOUBLE PRECISION NOT NULL,"
            ,"number_of_trades INTEGER NOT NULL,"
            ,"taker_buy_base_asset_volume DOUBLE PRECISION NOT NULL,"
            ,"taker_buy_quote_asset_volume DOUBLE PRECISION NOT NULL,"
            ,"ignore DOUBLE PRECISION);"
            };
        
        auto create_query = bypto::common::utils::intercalate(" ",parts);
        conn->execute(create_query);
    }

    void storeKlines(common::types::pgconn_t &conn, std::vector<Kline> &klines) {
        std::vector<std::string> field_list = 
            {"open_time","open","high"
            ,"low","close","volume"
            ,"close_time","quote_asset_volume","number_of_trades"
            ,"taker_buy_base_asset_volume","taker_buy_quote_asset_volume","ignore"
            };
        auto fields = common::utils::intercalate(",",field_list);
        std::vector<std::string> value_list =
            {"$1","$2","$3"
            ,"$4","$5","$6"
            ,"$7","$8","$9"
            ,"$10","$11","$12"
            };

        auto values = common::utils::intercalate(",",value_list);
        conn->prepare( "insert_kline", "INSERT INTO klines ("+fields+") VALUES ("+values+")");
        for(auto &kl : klines) {
            conn->execute("insert_kline"
                        ,kl.m_open_time,kl.m_open,kl.m_high
                        ,kl.m_low,kl.m_close,kl.m_volume
                        ,kl.m_close_time,kl.m_quote_asset_volume,kl.m_number_of_trades
                        ,kl.m_taker_buy_base_asset_volume,kl.m_taker_buy_quote_asset_volume,kl.m_ignore
                        );
        }
    }

    std::vector<Kline> loadKlines(common::types::pgconn_t &conn,time_t open_time,time_t close_time) {
        auto results = conn->execute(
                             "SELECT * FROM klines WHERE open_time >= $1 AND close_time <= $2"
                            ,open_time,close_time);
        std::vector<Kline> klines;
        for(auto &row : results) {
            Kline kl = { row["open_time"].as<time_t>()
                       , row["open"].as<long double>()
                       , row["high"].as<long double>()
                       , row["low"].as<long double>()
                       , row["close"].as<long double>()
                       , row["volume"].as<long double>()
                       , row["close_time"].as<time_t>()
                       , row["quote_asset_volume"].as<long double>()
                       , row["number_of_trades"].as<int>()
                       , row["taker_buy_base_asset_volume"].as<long double>()
                       , row["taker_buy_quote_asset_volume"].as<long double>()
                       , row["ignore"].as<long double>()
                       };
            klines.push_back(kl);
        }

        return klines;
    }

}