#pragma once

#include "bypto/common/types.h"
#include "bypto/order/types.h"
#include "bypto/order/order.h"
#include "bypto/order/order_type.h"
#include <optional>

namespace bypto::order {

    struct Partial {
        std::string m_order_type;
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

    template<OType T1,OType T2 = OType::Unit>
    FillResult<Order<T2>> orderToFill(Price p,Order<T1> o,std::optional<Order<T2>> m_new_order = std::nullopt) {
        return FillResult<Order<T2>>{o.m_ot.get_name(),p,o.m_sym,o.m_qty,o.m_pos,m_new_order};
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

    std::optional<FillResult<Order<OType::Unit>>> fill(Order<OType::Market> m,Price p);
    std::optional<FillResult<Order<OType::Unit>>> fill(Order<OType::Limit> m,Price p);
    std::optional<FillResult<Order<OType::Market>>> fill(Order<OType::StopLoss> m,Price p);
    std::optional<FillResult<Order<OType::Limit>>> fill(Order<OType::StopLossLimit> m,Price p);
    std::optional<FillResult<Order<OType::Market>>> fill(Order<OType::TakeProfit> m,Price p);
    std::optional<FillResult<Order<OType::Limit>>> fill(Order<OType::TakeProfitLimit> m,Price p);
    std::optional<FillResult<Order<OType::Unit>>> fill(Order<OType::LimitMaker> m,Price p);
}