#include "bypto/order/generic.h"
#include "bypto/order/order.h"
#include "bypto/order/fill.h"
#include "bypto/order/order_type.h"

#include <ostream>
#include <variant>

namespace bypto::order {

    GenericOrder::GenericOrder(Order<OType::Unit> o)
                              : m_sym(o.m_sym)
                              , m_qty(o.m_qty)
                              , m_pos(o.m_pos)
                              , m_type("Unit") {};

    GenericOrder::GenericOrder(Order<OType::Market> m)
                              : m_sym(m.m_sym)
                              , m_qty(m.m_qty)
                              , m_pos(m.m_pos)
                              , m_base_or_quote(m.m_ot.m_boq)
                              , m_generic_fill(map_generic(m))
                              , m_type("Market") {};

    GenericOrder::GenericOrder(Order<OType::Limit> m)
                              : m_sym(m.m_sym)
                              , m_qty(m.m_qty)
                              , m_pos(m.m_pos)
                              , m_limit(m.m_ot.m_limit)
                              , m_generic_fill(map_generic(m))
                              , m_type("Limit") {};

    GenericOrder::GenericOrder(Order<OType::StopLoss> m)
                              : m_sym(m.m_sym)
                              , m_qty(m.m_qty)
                              , m_pos(m.m_pos)
                              , m_stoptake(m.m_ot.m_stop)
                              , m_generic_fill(map_generic(m))
                              , m_type("StopLoss") {};

    GenericOrder::GenericOrder(Order<OType::StopLossLimit> m)
                              : m_sym(m.m_sym)
                              , m_qty(m.m_qty)
                              , m_pos(m.m_pos)
                              , m_stoptake(m.m_ot.m_stop)
                              , m_limit(m.m_ot.m_limit)
                              , m_generic_fill(map_generic(m))
                              , m_type("StopLossLimit") {};

    GenericOrder::GenericOrder(Order<OType::TakeProfit> m)
                              : m_sym(m.m_sym)
                              , m_qty(m.m_qty)
                              , m_pos(m.m_pos)
                              , m_stoptake(m.m_ot.m_take)
                              , m_generic_fill(map_generic(m))
                              , m_type("TakeProfit") {};

    GenericOrder::GenericOrder(Order<OType::TakeProfitLimit> m)
                              : m_sym(m.m_sym)
                              , m_qty(m.m_qty)
                              , m_pos(m.m_pos)
                              , m_stoptake(m.m_ot.m_take)
                              , m_limit(m.m_ot.m_limit)
                              , m_generic_fill(map_generic(m))
                              , m_type("TakeProfitLimit") {};

    GenericOrder::GenericOrder(Order<OType::LimitMaker> m)
                              : m_sym(m.m_sym)
                              , m_qty(m.m_qty)
                              , m_pos(m.m_pos)
                              , m_type("LimitMaker") {};
}