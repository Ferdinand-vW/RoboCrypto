#pragma once

#include "bypto/common/types.h"
#include "bypto/order/order.h"
#include <map>

namespace bypto::account {
    using namespace bypto::common::types;

    class Account {
        private:
            std::map<std::string,long double> m_funds;
            // std::map<int,bypto::order::Order> m_outstanding_orders;

        public:
            Account() = default;
            Account(std::map<std::string,long double> funds);
            std::map<std::string,long double> get_funds() const;
            //ideally we would convert all currencies in @m_funds to a single currency,
            //but we cannot guarantee that the given price_map has enough data to perform such conversion
            //For now we will do a one-pass conversion. E.g. if we maintain the following funds:
            // {(BTC,1),(USDT,100),(BUSD,200)}
            //and we are given the following price map
            // {(BTCUSDT,10000),(USD)}
            Error<Value> value(std::string ccy,std::map<Symbol,long double> price_map) const;
            void add_fund(std::string ccy,long double qty);
            void add_funds(const std::map<std::string,long double> &funds);

            long double get_quantity(std::string ccy) const;
    };

    std::ostream& operator<<(std::ostream&os, const Account &acc);
}