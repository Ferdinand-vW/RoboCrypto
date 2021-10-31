#pragma once

#include "bypto/common/types.h"
#include "bypto/order/types.h"
#include "bypto/order/order.h"
#include "bypto/order/order_type.h"
#include <optional>

namespace bypto::order {
    struct Unit {};

    struct Partial {
        Price m_price;
        Symbol m_sym;
        Quantity m_qty;
        Position m_pos;
    };

    template<typename T>
    struct FillResult {
        Partial m_partial;
        std::optional<T> m_new_order;
    };

    template<typename T1,typename T2 = Unit>
    FillResult<Order<T2>> orderToFill(Price p,Order<T1> o,std::optional<Order<T2>> m_new_order = std::nullopt) {
        return FillResult<Order<T2>>{p,o.m_sym,o.m_qty,o.m_pos,m_new_order};
    }

    template<typename T>
    std::ostream& operator<<(std::ostream &os,const FillResult<T> &fr) {
        os << "FillResult {";
        os << "m_price=" << fr.m_partial.m_price << ", ";
        os << "m_sym=" << fr.m_partial.m_sym << ", ";
        os << "m_qty=" << fr.m_partial.m_qty << ", ";
        os << "m_pos=" << fr.m_partial.m_pos << ", ";
        os << "m_new_order=" << fr.m_new_order << "}";
        return os;
    }

    std::optional<FillResult<Order<Unit>>> fill(Order<Market> m,Price p);
    std::optional<FillResult<Order<Unit>>> fill(Order<Limit> m,Price p);
    std::optional<FillResult<Order<Market>>> fill(Order<StopLoss> m,Price p);
    std::optional<FillResult<Order<Limit>>> fill(Order<StopLossLimit> m,Price p);
    std::optional<FillResult<Order<Market>>> fill(Order<TakeProfit> m,Price p);
    std::optional<FillResult<Order<Limit>>> fill(Order<TakeProfitLimit> m,Price p);
    std::optional<FillResult<Order<Unit>>> fill(Order<LimitMaker> m,Price p);
}