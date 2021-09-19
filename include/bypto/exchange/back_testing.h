#pragma once

#include "bypto/exchange.h"
#include "bypto/order.h"

namespace bypto::exchange::back_testing {

    class BackTesting : public Exchange {
        public:
            int execute_order(order::GenericOrder go);
            long double fetch_price();
            void cancel_order(int o_id);
    };
}