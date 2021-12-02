#pragma once

#include "bypto/common/types.h"
#include "bypto/order/types.h"
#include "bypto/order/order.h"
#include "bypto/order/order_type.h"
#include "bypto/order/generic.h"
#include <optional>

namespace bypto::order {

    struct Partial {
        Price m_price;
        GenericOrder m_order;
    };

    template<typename T>
    struct FillResult {
        Partial m_partial;
        std::optional<T> m_new_order;
    };

    template<OType T1,OType T2 = OType::Unit>
    FillResult<Order<T2>> orderToFill(Price p,Order<T1> o,std::optional<Order<T2>> m_new_order = std::nullopt) {
        return FillResult<Order<T2>>{Partial{p,GenericOrder(o)},m_new_order};
    }

    template<OType T>
    auto map_generic(Order<T> o) {
        auto f = [o](auto p) -> std::optional<FillResult<GenericOrder>> {
            auto fr_opt = fill(o,p); //
            if(fr_opt) {
                auto fr = fr_opt.value();
                auto new_order_opt = fr.m_new_order;
                if(new_order_opt) {
                    GenericOrder go(new_order_opt.value());
                    return FillResult<GenericOrder>{fr.m_partial,go};
                } else {
                    return FillResult<GenericOrder>{fr.m_partial,std::nullopt};
                }
            }
            return std::nullopt;
        };

        return f;
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