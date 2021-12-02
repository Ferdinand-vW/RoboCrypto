#include "bypto/order/generic.h"
#include "bypto/order/order.h"
#include "bypto/order/fill.h"
#include "bypto/order/order_type.h"

#include <ostream>
#include <variant>

namespace bypto::order {

    GenericOrder::GenericOrder(OType type,Symbol sym,Quantity qty,Position pos)
                              : m_type(type)
                              , m_sym(sym)
                              , m_qty(qty)
                              , m_pos(pos) {};

    GenericOrder::GenericOrder(Order<OType::Unit> o)
                              : m_sym(o.m_sym)
                              , m_qty(o.m_qty)
                              , m_pos(o.m_pos)
                              , m_type(OType::Unit) {};

    GenericOrder::GenericOrder(Order<OType::Market> m)
                              : m_sym(m.m_sym)
                              , m_qty(m.m_qty)
                              , m_pos(m.m_pos)
                              , m_base_or_quote(m.m_ot.m_boq)
                              , m_generic_fill(map_generic(m))
                              , m_type(OType::Market) {};

    GenericOrder::GenericOrder(Order<OType::Limit> m)
                              : m_sym(m.m_sym)
                              , m_qty(m.m_qty)
                              , m_pos(m.m_pos)
                              , m_limit(m.m_ot.m_limit)
                              , m_generic_fill(map_generic(m))
                              , m_type(OType::Limit) {};

    GenericOrder::GenericOrder(Order<OType::StopLoss> m)
                              : m_sym(m.m_sym)
                              , m_qty(m.m_qty)
                              , m_pos(m.m_pos)
                              , m_stoptake(m.m_ot.m_stop)
                              , m_generic_fill(map_generic(m))
                              , m_type(OType::StopLoss) {};

    GenericOrder::GenericOrder(Order<OType::StopLossLimit> m)
                              : m_sym(m.m_sym)
                              , m_qty(m.m_qty)
                              , m_pos(m.m_pos)
                              , m_stoptake(m.m_ot.m_stop)
                              , m_limit(m.m_ot.m_limit)
                              , m_generic_fill(map_generic(m))
                              , m_type(OType::StopLossLimit) {};

    GenericOrder::GenericOrder(Order<OType::TakeProfit> m)
                              : m_sym(m.m_sym)
                              , m_qty(m.m_qty)
                              , m_pos(m.m_pos)
                              , m_stoptake(m.m_ot.m_take)
                              , m_generic_fill(map_generic(m))
                              , m_type(OType::TakeProfit) {};

    GenericOrder::GenericOrder(Order<OType::TakeProfitLimit> m)
                              : m_sym(m.m_sym)
                              , m_qty(m.m_qty)
                              , m_pos(m.m_pos)
                              , m_stoptake(m.m_ot.m_take)
                              , m_limit(m.m_ot.m_limit)
                              , m_generic_fill(map_generic(m))
                              , m_type(OType::TakeProfitLimit) {};

    GenericOrder::GenericOrder(Order<OType::LimitMaker> m)
                              : m_sym(m.m_sym)
                              , m_qty(m.m_qty)
                              , m_pos(m.m_pos)
                              , m_type(OType::LimitMaker) {};

    void GenericOrder::set_base_or_quote(std::optional<BaseOrQuote> boq) {
        m_base_or_quote = boq;
    }
    void GenericOrder::set_time_in_force(std::optional<TimeInForce> tif) {
        m_time_in_force = tif;
    }
    void GenericOrder::set_stoptake(std::optional<Price> stop) {
        m_stoptake = stop;
    }
    void GenericOrder::set_limit(std::optional<Price> limit) {
        m_limit = limit;
    }

    std::ostream& operator<<(std::ostream&os,const GenericOrder& go) {
        os << "GenericOrder {";
        os << "m_type=" << go.m_type << ",";
        os << "m_sym=" << go.m_sym << ",";
        os << "m_pos=" << go.m_pos << ",";
        os << "m_qty=" << go.m_qty << ",";
        if(go.m_time_in_force) {
            os << "m_time_in_force=" << go.m_time_in_force.value() << ",";
        }
        
        if(go.m_limit) {
            os << "m_limit=" << go.m_limit.value() << ",";
        }

        if(go.m_stoptake) {
            os << "m_stoptake=" << go.m_stoptake.value() << ",";
        }

        if(go.m_base_or_quote) {
            os << "m_base_or_quote=" << go.m_base_or_quote.value() << ",";
        }

        os << "}";

        return os;
    }
}