#include "bypto/order.h"
#include "bypto/order_type.h"
#include <ostream>

namespace bypto::order {

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

    std::ostream& operator<<(std::ostream &os,const Position &p) {
        switch(p) {
            case Buy: os << "Buy"; break;
            case Sell: os << "Sell"; break;
        }

        return os;
    }

}