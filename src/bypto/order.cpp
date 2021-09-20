#include "bypto/order.h"
#include "bypto/order_type.h"
#include <ostream>

namespace bypto::order {

    std::ostream& operator<<(std::ostream &os,const Order &o) {
        os << "Order {";
        os << "m_symbol="     << o.m_symbol     << ",";
        os << "m_position="   << o.m_position   << ",";
        os << "m_order_type=" << o.m_order_type << "}";
        return os;
    }

    GenericOrderInfo::GenericOrderInfo(order_type::Market m)
                                      : m_quantity(m.m_quantity)
                                      , m_base_or_quote(m.m_boq) {};

    GenericOrderInfo::GenericOrderInfo(order_type::Limit l)
                                      : m_quantity(l.m_quantity)
                                      , m_price(l.m_price)
                                      , m_time_in_force(l.m_time_in_force) {};

    GenericOrderInfo::GenericOrderInfo(order_type::StopLoss sl)
                                      : m_quantity(sl.m_quantity)
                                      , m_stop_price(sl.m_stop_price) {};

    GenericOrderInfo::GenericOrderInfo(order_type::StopLossLimit sll)
                                      : m_quantity(sll.m_quantity)
                                      , m_price(sll.m_price)
                                      , m_stop_price(sll.m_stop_price)
                                      , m_time_in_force(sll.m_time_in_force) {};

    GenericOrderInfo::GenericOrderInfo(order_type::TakeProfit tp)
                                      : m_quantity(tp.m_quantity)
                                      , m_stop_price(tp.m_stop_price)  {};

    GenericOrderInfo::GenericOrderInfo(order_type::TakeProfitLimit tpl)
                                      : m_quantity(tpl.m_quantity)
                                      , m_price(tpl.m_price)
                                      , m_stop_price(tpl.m_stop_price)
                                      , m_time_in_force(tpl.m_time_in_force) {};

    GenericOrderInfo::GenericOrderInfo(order_type::LimitMaker lm)
                                      : m_quantity(lm.m_quantity)
                                      , m_price(lm.m_price) {};

    GenericOrderInfo GenericOrderInfo::fromOrderType(order_type::OrderType ot) {
        return std::visit(common::utils::overload{
            [](order_type::Market m) { return GenericOrderInfo(m); },
            [](order_type::Limit l) { return GenericOrderInfo(l);},
            [](order_type::StopLoss sl) { return GenericOrderInfo(sl); },
            [](order_type::StopLossLimit sll) { return GenericOrderInfo(sll); },
            [](order_type::TakeProfit tp) { return GenericOrderInfo(tp); },
            [](order_type::TakeProfitLimit tpl) { return GenericOrderInfo(tpl); },
            [](order_type::LimitMaker lm) { return GenericOrderInfo(lm); }
        },ot);
    }                                                                                                                                                  

    std::ostream& operator<<(std::ostream &os,const Position &p) {
        switch(p) {
            case Buy: os << "Buy"; break;
            case Sell: os << "Sell"; break;
        }

        return os;
    }

    GenericOrder::GenericOrder(Order order) : m_symbol(order.m_symbol)
                                            , m_position(order.m_position)
                                            , m_order_info(GenericOrderInfo::fromOrderType(order.m_order_type)) 
                                            {};

}