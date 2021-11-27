#include "bypto/exchange/backtest.h"
#include "bypto/account.h"
#include "bypto/common/math.h"
#include "bypto/common/utils.h"
#include "bypto/data/binance.h"
#include "bypto/data/kline.h"
#include "bypto/data/price.h"
#include "bypto/exchange.h"
#include "bypto/order/order.h"
#include "bypto/order/order_type.h"
#include <stdexcept>
#include <string>

namespace bypto::exchange {
    
    BackTest::BackTest(BackTestParams &&prms) 
                              :m_symbol(prms.m_sym)
                              ,m_klines(std::move(prms.m_klines)) {
        m_account.add_fund(prms.m_sym.base(),prms.m_base_qty);
        m_account.add_fund(prms.m_sym.quote(),prms.m_quote_qty);
        if(!prms.m_start_time) {
            m_curr_time = m_klines.front().get_time();
        } else {
            m_curr_time = prms.m_start_time.value();
        }
        if(!prms.m_tick_rate) {
            auto etick_rate = infer_tick_rate(m_klines);
            if(etick_rate.isLeft()) { throw std::invalid_argument(etick_rate.left()); }
            std::cout << "Inferred tick rate: " << etick_rate.right() << std::endl;
            m_tick_rate = etick_rate.right();
        }
    };

    //common error message
    std::string err_his_data() { return std::string("no remaining historical data"); }

    Error<account::Account> BackTest::get_account_info() {
        return m_account;
    }

    //We always buy/sell from the perspective of the base currency
    void BackTest::update_account(order::Partial p) {
        switch(p.m_pos) {
            case order::Position::Buy:
                //sym=BTCUSDT
                //price = 1 / actual
                //BTC_qty:qty
                //USDT_qty:qty/price
                m_account.add_fund(p.m_sym.base(), p.m_qty);
                m_account.add_fund(p.m_sym.quote(), -p.m_qty / p.m_price);
            break;
            case order::Position::Sell:
                std::cout << p.m_sym << " " << p.m_qty << " " << p.m_price << std::endl;
                m_account.add_fund(p.m_sym.base(), -p.m_qty);
                m_account.add_fund(p.m_sym.quote(), p.m_qty * p.m_price);
            break;
        }
    }

    Error<std::map<Symbol,long double>> BackTest::get_price_map(time_t t) {
        auto prices = pricesFromKlines(m_klines);

        auto price = 0;
        if(t == 0) {
            auto mlast = prices.back_opt();
            if(mlast) { 
                price = mlast->get_price();
            }
            else { return std::string("No klines data available.");}
        } else {
            auto klines = prices.most_recent(t);
            auto mfirst = klines.front_opt();
            if(!mfirst.has_value()) {
                return std::string("No klines data available.");
            } else {
                price = mfirst.value().get_price();
            }
        }

        auto sym_price = std::make_tuple(m_symbol,price);
        std::map<Symbol,long double> pm;
        pm.insert({m_symbol,price});
        return pm;

    }

    Error<long double> BackTest::fetch_price(Symbol _s) {
        auto prices = pricesFromKlines(m_klines);

        auto mkline = prices.index_opt(m_kline_index);
        if(!mkline.has_value()) { return err_his_data(); }

        auto kline = mkline.value();
        return common::math::interpolate(kline.m_open_time,kline.m_open
                                        ,kline.m_close_time,kline.m_close
                                        ,m_curr_time);
    }

    Error<bool> BackTest::cancel_order(int o_id) {
        m_outstanding.erase(o_id);

        return true;
    }

    time_t BackTest::get_current_time() {
        return m_curr_time;
    }

    Error<bool> BackTest::tick_once() {
        auto prices = pricesFromKlines(m_klines);
        if (m_klines.size() <= 0) { return false; }
        if(m_kline_index >= m_klines.size()) { return false; } // we ran out of data so signal to stop

        auto kline = prices[m_kline_index];
        //if we're past the time of the current kline
        //then we should move to the next
        if (kline.m_close_time < m_curr_time) {
            m_kline_index++;
            auto m_next = prices.index_opt(m_kline_index);
            //if current kline was last then return
            if(!m_next.has_value()) { return false; }
            else { kline = m_next.value(); }
        }

        long double curr_price = kline.m_close;

        // 1) loop over each outstanding order
        // 2) if order cannot be filled go to next, otherwise continue to (3)
        // 3a) if order spawned a new order, then replace current with new
        // 3b) otherwise execute order and update account
        // 4) delete the executed order
        for(auto o = m_outstanding.begin(); o != m_outstanding.end() ; ) { //(1)
            auto opt_fr = o->second.m_generic_fill(curr_price); //(2)
            if(opt_fr) { // (2)
               
                auto fr = opt_fr.value();
                if(fr.m_new_order) { // (3)
                    o->second = fr.m_new_order.value(); //(3.a)
                } else { // store result
                    update_account(fr.m_partial); //(3.b)
                }

                o = m_outstanding.erase(o); // (4)
            } else {
                ++o;
            }
        }

        m_curr_time = add_time(m_curr_time, m_tick_rate);

        return true;

    }

    Klines_t BackTest::get_historical_prices(time_t start,time_t end) {
        return pricesFromKlines(m_klines).time_interval(start, end);
    }

    Klines_t BackTest::get_all_historical() {
        return pricesFromKlines(m_klines);
    }

    std::stringstream BackTest::get_funds_csv() const {

    }

    std::stringstream BackTest::get_pnls_csv() const {

    }

    std::stringstream BackTest::get_orders_csv() const {

    }

    std::stringstream BackTest::get_prices_csv() const {

    }
}