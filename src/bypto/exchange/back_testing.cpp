#include "bypto/exchange/back_testing.h"
#include "bypto/account.h"
#include "bypto/common/math.h"
#include "bypto/common/utils.h"
#include "bypto/data/kline.h"
#include "bypto/data/price.h"
#include "bypto/exchange.h"
#include "bypto/order_type.h"
#include "bypto/strategy/ma.h"
#include <string>

namespace bypto::exchange {
    
    Exchange<ExchangeType::BackTest,PriceSource::Kline>::Exchange(Symbol symbol
                                                   ,Quantity base_fund,Quantity quote_fund
                                                   ,time_t tick_rate,data::price::Klines_t &&klines) 
                                                   :m_symbol(symbol)
                                                   ,m_tick_rate(tick_rate)
                                                   ,m_klines(std::move(klines)) {
        m_account.add_fund(symbol.base(),base_fund);
        m_account.add_fund(symbol.quote(),quote_fund);
                                                       
    };

    //common error message
    std::string err_his_data() { return std::string("no remaining historical data"); }

    Error<int> Exchange<ExchangeType::BackTest,PriceSource::Kline>::execute_order(order::Order go) {
        m_outstanding.insert({m_counter,go});
        m_counter++;
        return m_counter - 1;
    }

    Error<account::Account> Exchange<ExchangeType::BackTest,PriceSource::Kline>::get_account_info() {
        return m_account;
    }

    Error<Value> BackTestExchange::get_account_value(time_t t) {
        using namespace std::literals::string_literals;
    
        auto quote_price = 0;
        if(t == 0) {
            auto mlast = m_klines.back_opt();
            if(mlast) { 
                quote_price = mlast->get_quote_price();
            }
            else { return "No klines data available."s;}
        } else {
            auto klines = m_klines.most_recent(t);
            if(klines.size() < 0) {
                return "No klines data available."s;
            } else {
                auto kline = klines.front();
                quote_price = kline.get_quote_price();
            }
        }

        Value base_value(m_symbol.base(),m_account.get_quantity(m_symbol.base()));
        auto quote_qty = m_account.get_quantity(m_symbol.quote());
        Value quote_to_base(m_symbol.base(),quote_price * quote_qty);

        return base_value.add(quote_to_base);
    }

    Error<long double> Exchange<ExchangeType::BackTest,PriceSource::Kline>::fetch_price(Symbol _s) {
        auto mkline = m_klines.index_opt(m_kline_index);
        if(!mkline.has_value()) { return err_his_data(); }

        auto kline = mkline.value();
        return common::math::interpolate(kline.m_open_time,kline.m_open
                                        ,kline.m_close_time,kline.m_close
                                        ,m_curr_time);
    }

    Error<bool> Exchange<ExchangeType::BackTest,PriceSource::Kline>::cancel_order(int o_id) {
        m_outstanding.erase(o_id);

        return true;
    }

    Error<bool> Exchange<ExchangeType::BackTest,PriceSource::Kline>::tick_once() {
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

    std::span<data::price::Kline_t> Exchange<ExchangeType::BackTest,PriceSource::Kline>::historical_data(time_t period) {
        return m_klines.most_recent(period);
    }
}