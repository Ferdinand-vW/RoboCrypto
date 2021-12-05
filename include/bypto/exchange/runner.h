#pragma once

#include "bypto/account.h"
#include "bypto/data/price.h"
#include "bypto/exchange.h"
#include "bypto/strategy.h"

namespace bypto::exchange {

template <typename T, PriceSource P> class Runner {
  public:
    Runner(Exchange<T, P> &e) : m_exchange(e) {}

    template <typename S> Error<bool> run(Symbol sym, strategy::Strategy<S, P> &strat) {
        bool cont = true;
        // starting time point of historical data
        time_t hp_time = m_exchange.get_current_time();
        while (cont) {
            // we require account info to know about current
            // funds of assets that we own
            auto eacc = m_exchange.get_account_info();
            if (eacc.isLeft()) {
                return eacc.left();
            }
            auto acc = eacc.right();

            auto total_base_qty = acc.get_quantity(sym.base());
            auto total_quote_qty = acc.get_quantity(sym.quote());

            // we wish to spend a maximum of 1% capital in either currency
            auto spendable_base = total_base_qty / 100;   // 1%
            auto spendable_quote = total_quote_qty / 100; // 1%

            auto now = m_exchange.get_current_time();
            // get set of prices between "now" and starting point. TODO: Starting point should probably be
            // max(hp_time,now-4h)
            auto prices = m_exchange.get_historical_prices(hp_time, now);

            // only if enough data is present can we attempt to apply a strategy
            if (strat.has_enough_data(prices)) {
                auto emorder = strat.make_decision(now, spendable_base, spendable_quote, prices);

                if (emorder.isLeft()) {
                    return emorder.left();
                } else if (emorder.right()) {
                    auto order = emorder.right().value();
                    m_exchange.execute_order(order); // is only executed at tick
                }
            }

            // collect data, execute outstanding orders
            auto esucc = m_exchange.tick_once();
            if (esucc.isLeft()) {
                return esucc;
            } else {
                cont = esucc.right();
            }
        }
        return true;
    }

  private:
    Exchange<T, P> &m_exchange;
};
} // namespace bypto::exchange