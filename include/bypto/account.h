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
            void add_fund(std::string ccy,long double qty);

            long double get_quantity(std::string ccy) {
                if(m_funds.contains(ccy)) {
                    return m_funds[ccy];
                } else {
                    return 0;
                }
            }
    };
}