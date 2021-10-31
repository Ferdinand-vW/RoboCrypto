#pragma once

#include "bypto/common/types.h"
#include "bypto/order/base.h"
#include "bypto/order/order.h"
#include "bypto/order/types.h"

namespace bypto::order {
    typedef std::variant<Order<Market>,Order<Limit>> MarketOrLimit;
    struct FillResult {
        Price m_price;
        Symbol m_symbol;
        Quantity m_qty;
        Position m_pos;
        std::optional<MarketOrLimit> m_new_order;
    };

    std::optional<FillResult> fill(Market m);
    std::optional<FillResult> fill(Limit m);
    std::optional<FillResult> fill(StopLoss m);
    std::optional<FillResult> fill(StopLossLimit m);
    std::optional<FillResult> fill(TakeProfit m);
    std::optional<FillResult> fill(TakeProfitLimit m);
    std::optional<FillResult> fill(LimitMaker m);

    template<typename T>
    std::optional<FillResult> fill_order(Order<T> o) {
        return fill(o.get_order_type());
    }
}