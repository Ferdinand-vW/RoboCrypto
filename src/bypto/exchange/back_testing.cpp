#include "bypto/exchange/back_testing.h"

#include "bypto/common/math.h"
#include "bypto/common/utils.h"
#include "bypto/data/binance/klines.h"
#include "bypto/order_type.h"

namespace bypto::exchange {
    
    Exchange<BackTest>::Exchange(std::string symbol,int tick_rate,std::deque<data::binance::klines::Kline> klines) 
                            : m_symbol(symbol)
                            ,m_tick_rate(tick_rate)
                            ,m_klines(klines) {};

    int Exchange<BackTest>::execute_order(order::Order go) {
        m_outstanding.insert({m_counter,go});
        m_counter++;
        return m_counter - 1;
    }

    long double Exchange<BackTest>::fetch_price() {
        auto kline = m_klines.front();
        return common::math::interpolate(kline.m_open_time,kline.m_open
                                        ,kline.m_close_time,kline.m_close
                                        ,m_curr_time);
    }

    void Exchange<BackTest>::cancel_order(int o_id) {
        m_outstanding.erase(o_id);
    }

    bool Exchange<BackTest>::tick_once() {
        if (m_klines.size() <= 0) { return false; }

        m_curr_time += m_tick_rate;

        auto kline = m_klines.front();
        //if we're past the time of the current kline
        //then we should move to the next
        if (kline.m_close_time < m_curr_time) {
            m_klines.pop_front();
            //if current kline was last then return
            if(m_klines.size() <= 0) { return false; }
            else { kline = m_klines.front(); }
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

    std::vector<Kline> Exchange<BackTest>::historical_data(time_t start, time_t end) {
        std::vector<Kline> klines;
        for(auto & kl : m_klines) {
            if (kl.m_open_time >= start && kl.m_close_time < end) {
                klines.push_back(kl);
            } 
        }
        return klines;
    }
}