#pragma once

#include "bypto/data/price.h"
#include "bypto/exchange.h"
#include "bypto/account.h"
#include "bypto/strategy.h"
#include "bypto/strategy/ma.h"

namespace bypto::exchange::runner {

    template<ExchangeType T,PriceSource P>
    class Runner {
        public:
            Runner(Exchange<T,P> e) 
                    : m_exchange(e) {}

            template<template<PriceSource> typename S>
            Error<bool> run(Symbol sym, strategy::Strategy<S,P> strat) {
                bool cont = true;
                time_t hp_time = m_exchange.get_current_time();
                while (cont) {
                    auto eacc = m_exchange.get_account_info();
                    if(eacc.isLeft()) { return eacc.left(); }

                    auto acc = eacc.right();

                    auto total_base_qty = acc.get_quantity(sym.base());
                    auto total_quote_qty = acc.get_quantity(sym.quote());

                    auto spendable_base = total_base_qty / 100; //1%
                    auto spendable_quote = total_quote_qty / 100; // 1%

                    auto now = m_exchange.get_current_time();
                    auto prices = m_exchange.get_historical_prices(hp_time);

                    auto emorder = strat.make_decision(now, spendable_base, spendable_quote, prices);

                    if(emorder.isLeft()) {
                        return emorder.left();
                    }
                    if(emorder.right()) {
                        auto order = emorder.right().value();
                        std::cout << "New order " << order.m_order_type;
                        m_exchange.execute_order(order);
                    }

                    auto esucc = m_exchange.tick_once();
                    if(esucc.isLeft()) { return esucc; }
                    else { cont = esucc.right(); }

                }

                return true;
            }

        private:
            Exchange<T,P> m_exchange;
    };

    typedef Runner<ExchangeType::BackTest,PriceSource::Kline> BackTestRunner;
}