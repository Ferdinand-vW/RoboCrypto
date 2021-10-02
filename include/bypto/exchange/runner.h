#pragma once

#include "bypto/exchange.h"
#include "bypto/account.h"
#include "bypto/strategy.h"

namespace bypto::exchange::runner {
    
    template<typename T>
    class Runner {
        public:
            Runner(Exchange<T> e,account::Account &acc) 
                    : m_exchange(e)
                    , m_account(acc) {}

            template <typename S>
            void run(S strat) {
                auto p = m_exchange.fetch_price();
                
                
                /**

                Given a price,account and historic price data we can do one of the following:

                - Cancel Order
                - Make new order (strategy should determine order type,position and qty)
                - Do Nothing

                **/
            }

        private:
            Exchange<T> m_exchange;
            account::Account m_account;
    };
}