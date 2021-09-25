#include "bypto/order.h"
#include "bypto/order_type.h"
#include <ostream>
#include <variant>

namespace bypto::order {

    std::ostream& operator<<(std::ostream &os,const Order &o) {
        os << "Order {";
        os << "m_symbol="     << o.m_symbol     << ",";
        os << "m_pos="   << o.m_pos   << ",";
        os << "m_order_type=" << o.m_order_type << "}";
        return os;
    }

    Order::Order(std::string symbol,order_type::Position pos,order_type::OrderType ot)
                : m_symbol(symbol)
                , m_pos(pos)
                , m_order_type(ot) {};

    std::optional<order_type::FillResult> Order::try_fill(Symbol symbol,Price price) {
        //we can't fill this order if the symbols don't match
        if(m_symbol != symbol) {
            return {};
        }

        auto opt_fr = std::visit(common::utils::overload{
            [&](order_type::Market m)            { return m.try_fill(m_pos,price); },
            [&](order_type::Limit l)             { return l.try_fill(m_pos,price); },
            [&](order_type::StopLoss sl)         { return sl.try_fill(m_pos,price); },
            [&](order_type::StopLossLimit sll)   { return sll.try_fill(m_pos,price); },
            [&](order_type::TakeProfit tp)       { return tp.try_fill(m_pos,price); },
            [&](order_type::TakeProfitLimit tpl) { return tpl.try_fill(m_pos,price); },
            [&](order_type::LimitMaker lm)       { return lm.try_fill(m_pos,price); }
        }, m_order_type);


        return opt_fr;
    }

    GenericOrderInfo::GenericOrderInfo(order_type::Market m)
                                      : m_qty(m.m_qty)
                                      , m_base_or_quote(m.m_boq) {};

    GenericOrderInfo::GenericOrderInfo(order_type::Limit l)
                                      : m_qty(l.m_qty)
                                      , m_price(l.m_price)
                                      , m_time_in_force(l.m_time_in_force) {};

    GenericOrderInfo::GenericOrderInfo(order_type::StopLoss sl)
                                      : m_qty(sl.m_qty)
                                      , m_stop_price(sl.m_stop_price) {};

    GenericOrderInfo::GenericOrderInfo(order_type::StopLossLimit sll)
                                      : m_qty(sll.m_qty)
                                      , m_price(sll.m_price)
                                      , m_stop_price(sll.m_stop_price)
                                      , m_time_in_force(sll.m_time_in_force) {};

    GenericOrderInfo::GenericOrderInfo(order_type::TakeProfit tp)
                                      : m_qty(tp.m_qty)
                                      , m_stop_price(tp.m_stop_price)  {};

    GenericOrderInfo::GenericOrderInfo(order_type::TakeProfitLimit tpl)
                                      : m_qty(tpl.m_qty)
                                      , m_price(tpl.m_price)
                                      , m_stop_price(tpl.m_stop_price)
                                      , m_time_in_force(tpl.m_time_in_force) {};

    GenericOrderInfo::GenericOrderInfo(order_type::LimitMaker lm)
                                      : m_qty(lm.m_qty)
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

    GenericOrder::GenericOrder(Order order) : m_symbol(order.m_symbol)
                                            , m_pos(order.m_pos)
                                            , m_order_info(GenericOrderInfo::fromOrderType(order.m_order_type)) 
                                            {};

}