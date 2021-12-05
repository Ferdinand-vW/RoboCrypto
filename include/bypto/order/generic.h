#pragma once

#include "bypto/order/order.h"
#include "bypto/order/order_type.h"
#include <optional>

namespace bypto::order {
template <typename T> struct FillResult;

class GenericOrder {

  public:
    OType m_type;

    Symbol m_sym;
    Quantity m_qty;
    Position m_pos;
    std::optional<TimeInForce> m_time_in_force;
    std::optional<Price> m_limit;
    std::optional<Price> m_stoptake;
    std::optional<BaseOrQuote> m_base_or_quote;
    void set_base_or_quote(std::optional<BaseOrQuote> boq);
    void set_time_in_force(std::optional<TimeInForce> tif);
    void set_stoptake(std::optional<Price> stop);
    void set_limit(std::optional<Price> limit);

    std::function<std::optional<FillResult<GenericOrder>>(Price)> m_generic_fill;

    GenericOrder(OType type, Symbol sym, Quantity qty, Position pos);
    GenericOrder(Order<OType::Unit> o);
    GenericOrder(Order<OType::Market> m);
    GenericOrder(Order<OType::Limit> l);
    GenericOrder(Order<OType::StopLoss> sl);
    GenericOrder(Order<OType::StopLossLimit> sll);
    GenericOrder(Order<OType::TakeProfit> tp);
    GenericOrder(Order<OType::TakeProfitLimit> tpl);
    GenericOrder(Order<OType::LimitMaker> lm);
};

std::ostream &operator<<(std::ostream &os, const GenericOrder &go);
} // namespace bypto::order