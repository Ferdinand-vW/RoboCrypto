#pragma once

#include "bypto/common/types.h"
#include "bypto/order.h"
#include <map>

namespace bypto::account {
    using namespace bypto::common::types;

    class Account {
        private:
            std::map<std::string,long double> m_funds;
            std::map<int,bypto::order::Order> m_outstanding_orders;

        public:
            void add_fund(std::string ccy,long double qty);
    };
}