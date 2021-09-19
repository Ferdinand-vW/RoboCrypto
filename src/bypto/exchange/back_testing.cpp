#include "bypto/exchange/back_testing.h"

#include "bypto/common/math.h"

namespace bypto::exchange::back_testing {
    
    BackTesting::BackTesting(std::string symbol,int tick_rate,std::deque<data::binance::klines::Kline> klines) 
                            : m_symbol(symbol)
                            ,m_tick_rate(tick_rate)
                            ,m_klines(klines) {};

    int BackTesting::execute_order(order::GenericOrder go) {
        m_outstanding.insert({m_counter,go});
        m_counter++;
        return m_counter - 1;
    }

    long double BackTesting::fetch_price() {
        auto kline = m_klines.front();
        return common::math::interpolate(kline.m_open_time,kline.m_open
                                        ,kline.m_close_time,kline.m_close
                                        ,m_curr_time);
    }

    void BackTesting::cancel_order(int o_id) {
        m_outstanding.erase(o_id);
    }

    bool BackTesting::tick_once() {
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

        //TODO: logic to see if any GenericOrders can be fulfilled

        return true;

    }
}