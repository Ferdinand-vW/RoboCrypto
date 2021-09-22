#pragma once

#include "bypto/order.h"

namespace bypto::exchange {
    
    class Exchange {
        public:
            virtual int execute_order(order::Order order) = 0;
            virtual long double fetch_price() = 0;
            virtual void cancel_order(int o_id) = 0;
            virtual bool tick_once() = 0;
    };
}