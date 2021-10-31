#pragma once

#include "bypto/common/types.h"
#include "bypto/order/base.h"

namespace bypto::order {
    class OrderInfo {
        virtual common::types::Symbol get_symbol() const = 0;
        virtual common::types::Quantity get_quantity() const = 0;
        virtual Position get_position() const = 0;
        virtual BaseOrQuote get_base_or_quote() const = 0;  
    };
}