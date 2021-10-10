#pragma once

#include "bypto/data/price.h"
#include "bypto/exchange.h"
#include "bypto/account.h"
#include "bypto/strategy.h"

namespace bypto::exchange::runner {

    template<ExchangeType T,PriceSource P>
    class Runner {
        public:
            Runner(Exchange<T,P> e) 
                    : m_exchange(e) {}

            template <strategy::Indicator S>
            Error<bool> run(Symbol sym, strategy::Strategy<S,P> strat) {
                auto acc = m_exchange.get_account_info();

                auto total_base_qty = acc.get_quantity(sym.base());
                auto total_quote_qty = acc.get_quantity(sym.quote());

                auto spendable_base = total_base_qty / 100; //1%
                auto spendable_quote = total_quote_qty / 100; // 1%

                auto now = m_exchange.get_current_time();
                auto prices = m_exchange.get_historical_prices();

                auto morder = strat.make_decision(now, spendable_base, spendable_quote, prices);

                if(morder) {
                    m_exchange.execute_order(morder.value());
                }

                auto succ = m_exchange.tick_once();
            }

        private:
            Exchange<T,P> m_exchange;
    };

    typedef Runner<ExchangeType::BackTest,PriceSource::Kline> BackTestRunner;
}