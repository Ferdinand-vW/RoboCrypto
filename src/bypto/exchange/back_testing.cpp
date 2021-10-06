#include "bypto/exchange/back_testing.h"
#include "bypto/common/math.h"
#include "bypto/common/utils.h"
#include "bypto/data/kline.h"
#include "bypto/data/price.h"
#include "bypto/order_type.h"
#include "bypto/strategy/ma.h"
#include <string>

namespace bypto::exchange {
    
    Exchange<BackTest,PriceSource::Kline>::Exchange(std::string symbol,int tick_rate,data::price::Klines_t &&klines) 
                            : m_symbol(symbol)
                            ,m_tick_rate(tick_rate)
                            ,m_klines(std::move(klines)) {};

    //common error message
    std::string err_his_data() { return std::string("no remaining historical data"); }

    Error<int> Exchange<BackTest,PriceSource::Kline>::execute_order(order::Order go) {
        m_outstanding.insert({m_counter,go});
        m_counter++;
        return m_counter - 1;
    }

    Error<long double> Exchange<BackTest,PriceSource::Kline>::fetch_price(Symbol _s) {
        auto mkline = m_klines.index_opt(m_kline_index);
        if(!mkline.has_value()) { return err_his_data(); }

        auto kline = mkline.value();
        return common::math::interpolate(kline.m_open_time,kline.m_open
                                        ,kline.m_close_time,kline.m_close
                                        ,m_curr_time);
    }

    Error<bool> Exchange<BackTest,PriceSource::Kline>::cancel_order(int o_id) {
        m_outstanding.erase(o_id);

        return true;
    }

    Error<bool> Exchange<BackTest,PriceSource::Kline>::tick_once() {
        if (m_klines.size() <= 0) { return err_his_data(); }

        m_curr_time += m_tick_rate;

        auto kline = m_klines[m_kline_index];
        //if we're past the time of the current kline
        //then we should move to the next
        if (kline.m_close_time < m_curr_time) {
            m_kline_index++;
            auto m_next = m_klines.index_opt(m_kline_index);
            //if current kline was last then return
            if(!m_next.has_value()) { return err_his_data(); }
            else { kline = m_next.value(); }
        }

        long double curr_price = kline.m_close;
        //TODO: logic to see if any outstanding orders can be filled
        for(auto &o : m_outstanding) {
            auto opt_fr = o.second.try_fill(m_symbol, curr_price);
            if(opt_fr) { // could fill order
                auto fr = opt_fr.value();
                if(fr.m_new_order) { //order triggered a new order
                    o.second.m_order_type = fr.m_new_order.value();
                } else { // store result
                    auto partial = order_type::fillToPartial(fr);
                    m_partials.insert({o.first,partial});
                }
            }
        }

        return true;

    }

    std::span<data::price::Kline_t> Exchange<BackTest,PriceSource::Kline>::historical_data(time_t period) {
        return m_klines.most_recent(period);
    }
}