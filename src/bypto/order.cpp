#include "bypto/order.h"
#include "bypto/order_type.h"
#include <ostream>

namespace bypto::order {

    std::ostream& operator<<(std::ostream &os,const Position &p) {
        switch(p) {
            case Buy: os << "Buy"; break;
            case Sell: os << "Sell"; break;
        }

        return os;
    }

    GenericOrderInfo GenericOrderInfo::from_order_type(order_type::Market m) {
        return GenericOrderInfo{{},m.m_quantity,{},{},m.m_boq};
    }

    GenericOrderInfo GenericOrderInfo::from_order_type(order_type::Limit l) {
        return GenericOrderInfo{l.m_time_in_force,l.m_quantity,l.m_price,{},{}};
    }

    GenericOrderInfo GenericOrderInfo::from_order_type(order_type::StopLoss sl) {
        return GenericOrderInfo{{},sl.m_quantity,{},sl.m_stop_price,{}};
    }

    GenericOrderInfo GenericOrderInfo::from_order_type(order_type::StopLossLimit sll) {
        return GenericOrderInfo{sll.m_time_in_force,sll.m_quantity,sll.m_price,sll.m_stop_price,{}};
    }

    GenericOrderInfo GenericOrderInfo::from_order_type(order_type::TakeProfit tp) {
        return GenericOrderInfo{{},tp.m_quantity,{},tp.m_stop_price,{}};
    }

    GenericOrderInfo GenericOrderInfo::from_order_type(order_type::TakeProfitLimit tpl) {
        return GenericOrderInfo{tpl.m_time_in_force,tpl.m_quantity,tpl.m_price,tpl.m_stop_price,{}};
    }

    GenericOrderInfo GenericOrderInfo::from_order_type(order_type::LimitMaker lm) {
        return GenericOrderInfo{{},lm.m_quantity,lm.m_price,{},{}};
    }

}