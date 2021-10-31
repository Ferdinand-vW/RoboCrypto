#include "bypto/exchange/back_testing.h"
#include "bypto/account.h"
#include "bypto/common/math.h"
#include "bypto/common/utils.h"
#include "bypto/data/binance.h"
#include "bypto/data/kline.h"
#include "bypto/data/price.h"
#include "bypto/exchange.h"
#include "bypto/order/order.h"
#include "bypto/order/order_type.h"
#include "bypto/strategy/ma.h"
#include <string>

namespace bypto::exchange {
    
    BackTestExchange::Exchange(Symbol symbol
                              ,Quantity base_fund,Quantity quote_fund
                              ,time_t start_time,time_unit tick_rate
                              ,std::vector<Kline_t> &&klines) 
                              :m_symbol(symbol)
                              ,m_curr_time(start_time)
                              ,m_tick_rate(tick_rate)
                              ,m_klines(std::move(klines)) {
        m_account.add_fund(symbol.base(),base_fund);
        m_account.add_fund(symbol.quote(),quote_fund);
    };

    //common error message
    std::string err_his_data() { return std::string("no remaining historical data"); }

    Error<account::Account> BackTestExchange::get_account_info() {
        return m_account;
    }

    void BackTestExchange::update_account(order::Partial p) {
        switch(p.m_pos) {
            case order::Position::Buy:
                m_account.add_fund(p.m_sym.to_string(), -p.m_qty);
            break;
            case order::Position::Sell:
            break;
        }
    }

    Error<Value> BackTestExchange::get_account_value(time_t t) {
        using namespace std::literals::string_literals;
        auto prices = pricesFromKlines(m_klines);

        auto quote_price = 0;
        if(t == 0) {
            auto mlast = prices.back_opt();
            if(mlast) { 
                quote_price = mlast->get_quote_price();
            }
            else { return "No klines data available."s;}
        } else {
            auto klines = prices.most_recent(t);
            auto mfirst = klines.front_opt();
            if(!mfirst.has_value()) {
                return "No klines data available."s;
            } else {
                quote_price = mfirst.value().get_quote_price();
            }
        }

        Value base_value(m_symbol.base(),m_account.get_quantity(m_symbol.base()));
        auto quote_qty = m_account.get_quantity(m_symbol.quote());
        Value quote_to_base(m_symbol.base(),quote_price * quote_qty);

        return base_value.add(quote_to_base);
    }

    Error<long double> BackTestExchange::fetch_price(Symbol _s) {
        auto prices = pricesFromKlines(m_klines);

        auto mkline = prices.index_opt(m_kline_index);
        if(!mkline.has_value()) { return err_his_data(); }

        auto kline = mkline.value();
        return common::math::interpolate(kline.m_open_time,kline.m_open
                                        ,kline.m_close_time,kline.m_close
                                        ,m_curr_time);
    }

    Error<bool> BackTestExchange::cancel_order(int o_id) {
        m_outstanding.erase(o_id);

        return true;
    }

    time_t BackTestExchange::get_current_time() {
        return m_curr_time;
    }

    Error<bool> BackTestExchange::tick_once() {
        auto prices = pricesFromKlines(m_klines);
        if (m_klines.size() <= 0) { return err_his_data(); }
        if(m_kline_index >= m_klines.size()) { return false; }// we ran out of data so signal to stop
        std::cout << "Time: " << pp_time(m_curr_time) << std::endl;
        m_curr_time = add_time(m_curr_time, m_tick_rate);

        std::cout << m_klines.size() << std::endl;
        auto kline = prices[m_kline_index];
        std::cout << m_kline_index <<std::endl;
        //if we're past the time of the current kline
        //then we should move to the next
        if (kline.m_close_time < m_curr_time) {
            m_kline_index++;
            auto m_next = prices.index_opt(m_kline_index);
            //if current kline was last then return
            if(!m_next.has_value()) { return err_his_data(); }
            else { kline = m_next.value(); }
        }

        long double curr_price = kline.m_close;
        //TODO: logic to see if any outstanding orders can be filled
        for(auto o = m_outstanding.begin(); o != m_outstanding.end() ; ) {
            auto opt_fr = o->second.m_generic_fill(curr_price);
            if(opt_fr) { // could fill 
                std::cout << "can fill oustanding:" << o->first << std::endl;
                auto fr = opt_fr.value();
                if(fr.m_new_order) { //order triggered a new order
                    o->second = fr.m_new_order.value();
                } else { // store result
                    update_account(fr.m_partial);
                }

                o = m_outstanding.erase(o);
            } else {
                ++o;
            }
        }

        return true;

    }

    Klines_t BackTestExchange::get_historical_prices(time_t start,time_t end) {
        return pricesFromKlines(m_klines).time_interval(start, end);
    }
}