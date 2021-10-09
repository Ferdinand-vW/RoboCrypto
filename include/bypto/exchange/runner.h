#pragma once

#include "bypto/data/price.h"
#include "bypto/exchange.h"
#include "bypto/account.h"
#include "bypto/strategy.h"

namespace bypto::exchange::runner {
    
    template<typename T,PriceSource P>
    class Runner {
        public:
            Runner(Exchange<T,P> e,account::Account &acc) 
                    : m_exchange(e)
                    , m_account(acc) {}

            template <strategy::Indicator S>
            void run(strategy::Strategy<S,P>) {
                //get account info from exchange
                //decide on acceptable base qty and qoute qty
                //call strategy
                //if no order then tick once
                //if new order then call exchange.execute_order

                auto succ = m_exchange.tick_once();
                
                
                /**

                Given a price,account and historic price data we can do one of the following:

                - Cancel Order
                - Make new order (strategy should determine order type,position and qty)
                - Do Nothing

                **/
            }

        private:
            Exchange<T,P> m_exchange;
            account::Account m_account;
    };
}