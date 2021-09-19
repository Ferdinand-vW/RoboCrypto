#pragma once

#include "bypto/order.h"

namespace bypto::exchange {
    
    class Exchange {
        public:
            virtual int execute_order(order::GenericOrder go) = 0;
            virtual long double fetch_price() = 0;
            virtual void cancel_order(int o_id) = 0;
    };
}