#include "bypto/data/binance.h"
#include "bypto/common/csv.h"
#include "bypto/common/types.h"
#include "bypto/common/utils.h"
#include "bypto/data/price.h"
#include "bypto/data/kline.h"
#include "bypto/data/prices.h"

namespace bypto::data::binance {

    price::Klines_t klinesToPrices(std::vector<price::Kline_t> &&klines) {
        if(klines.size() < 2) {
            throw "Klines data must contain at least 2 entries";
        }

        auto interval = klines[1].m_close_time - klines[0].m_close_time;
        auto eprices = price::Klines_t::CreatePrices(interval,std::move(klines));

        //klines data should be provided in consistent format
        if(eprices.isLeft()) {
            throw eprices.left();
        }

        return eprices.right();
    }

    price::Klines_t parseCSV(common::types::Symbol sym, std::istream &is) {
        typedef long double ld;
        //sadly type inference is unable to figure out what
        //the type of the header should be
        auto tpls = common::csv::parse
                        <time_t,ld,ld,ld,ld,ld
                        ,time_t,ld,long,ld,ld,ld>(is);

        auto toKline = [sym](auto &tpl) {
            return price::Kline_t
                { sym
                , std::get<0>(tpl)/1000 // binance timestamps are in milliseconds, time_t is in seconds
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

        std::vector<price::Kline_t> klines;
        std::transform(tpls.begin(),tpls.end(),std::back_inserter(klines),toKline);

        return klinesToPrices(std::move(klines));
    }

    void prepareTable(common::types::pgconn_t &conn) {
        using namespace std::string_literals;
        std::vector<std::string> parts = 
            {"CREATE TABLE IF NOT EXISTS klines ("s
            ,"kline_id SERIAL NOT NULL PRIMARY KEY,"
            ,"symbol VARCHAR(32) NOT NULL,"
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

    void storeKlines(common::types::pgconn_t &conn, price::Klines_t &klines) {
        std::vector<std::string> field_list = 
            { "symbol"
            ,"open_time","open","high"
            ,"low","close","volume"
            ,"close_time","quote_asset_volume","number_of_trades"
            ,"taker_buy_base_asset_volume","taker_buy_quote_asset_volume","ignore"
            };
        auto fields = common::utils::intercalate(",",field_list);
        std::vector<std::string> value_list =
            {"$1"
            ,"$2","$3","$4"
            ,"$5","$6","$7"
            ,"$8","$9","$10"
            ,"$11","$12","$13"
            };

        auto values = common::utils::intercalate(",",value_list);
        conn->prepare( "insert_kline", "INSERT INTO klines ("+fields+") VALUES ("+values+")");
        for(auto &kl : klines) {
            conn->execute("insert_kline"
                        ,kl.m_symbol.to_string()
                        ,kl.m_open_time,kl.m_open,kl.m_high
                        ,kl.m_low,kl.m_close,kl.m_volume
                        ,kl.m_close_time,kl.m_quote_asset_volume,kl.m_number_of_trades
                        ,kl.m_taker_buy_base_asset_volume,kl.m_taker_buy_quote_asset_volume,kl.m_ignore
                        );
        }
    }

    price::Klines_t loadKlines(common::types::pgconn_t &conn,time_t open_time,time_t close_time) {
        auto results = conn->execute(
                             "SELECT * FROM klines WHERE open_time >= $1 AND close_time <= $2"
                            ,open_time,close_time);
        std::vector<price::Kline_t> klines;
        for(auto &row : results) {
            price::Kline_t kl = { common::types::Symbol::from_string(row["symbol"].as<std::string>())
                       , row["open_time"].as<time_t>()
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


        return klinesToPrices(std::move(klines));
    }


}