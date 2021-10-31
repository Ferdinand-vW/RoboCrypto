#include "bypto/order/generic.h"
#include "bypto/order/order.h"
#include "bypto/order/fill.h"
#include "bypto/order/order_type.h"

#include <ostream>
#include <variant>

namespace bypto::order {

    GenericOrder::GenericOrder(Order<Unit> o)
                              : m_sym(o.m_sym)
                              , m_qty(o.m_qty)
                              , m_pos(o.m_pos) {};

    GenericOrder::GenericOrder(Order<Market> m)
                              : m_sym(m.m_sym)
                              , m_qty(m.m_qty)
                              , m_pos(m.m_pos)
                              , m_base_or_quote(m.m_ot.m_boq)
                              , m_generic_fill(map_generic(m)) {};

    GenericOrder::GenericOrder(Order<Limit> m)
                              : m_sym(m.m_sym)
                              , m_qty(m.m_qty)
                              , m_pos(m.m_pos)
                              , m_limit(m.m_ot.m_limit)
                              , m_generic_fill(map_generic(m)) {};

    GenericOrder::GenericOrder(Order<StopLoss> m)
                              : m_sym(m.m_sym)
                              , m_qty(m.m_qty)
                              , m_pos(m.m_pos)
                              , m_stoptake(m.m_ot.m_stop)
                              , m_generic_fill(map_generic(m)) {};

    GenericOrder::GenericOrder(Order<StopLossLimit> m)
                              : m_sym(m.m_sym)
                              , m_qty(m.m_qty)
                              , m_pos(m.m_pos)
                              , m_stoptake(m.m_ot.m_stop)
                              , m_limit(m.m_ot.m_limit)
                              , m_generic_fill(map_generic(m)) {};

    GenericOrder::GenericOrder(Order<TakeProfit> m)
                              : m_sym(m.m_sym)
                              , m_qty(m.m_qty)
                              , m_pos(m.m_pos)
                              , m_stoptake(m.m_ot.m_take)
                              , m_generic_fill(map_generic(m)) {};

    GenericOrder::GenericOrder(Order<TakeProfitLimit> m)
                              : m_sym(m.m_sym)
                              , m_qty(m.m_qty)
                              , m_pos(m.m_pos)
                              , m_stoptake(m.m_ot.m_take)
                              , m_limit(m.m_ot.m_limit)
                              , m_generic_fill(map_generic(m)) {};

    GenericOrder::GenericOrder(Order<LimitMaker> m)
                              : m_sym(m.m_sym)
                              , m_qty(m.m_qty)
                              , m_pos(m.m_pos) {};
}