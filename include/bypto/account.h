#pragma once

#include "bypto/common/types.h"
#include "bypto/order.h"
namespace bypto::account {
    using namespace bypto::common::types;

    class Account {
        private:
            std::map<Symbol,long double> m_holdings;
            std::map<int,bypto::order::Order> m_outstanding_orders;
    };
}