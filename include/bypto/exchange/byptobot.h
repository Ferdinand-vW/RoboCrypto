#pragma once

#include "bypto/exchange.h"
#include "bypto/account.h"

namespace bypto::exchange::byptobot {
    
    class ByptoBot {
        public:
            ByptoBot(Exchange e,account::Account &acc);

            void run();
    };
}