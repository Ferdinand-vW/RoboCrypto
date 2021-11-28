#pragma once

#include "bypto/common/utils.h"
#include "bypto/common/types.h"
#include "bypto/data/price.h"
#include "bypto/order/types.h"
#include "bypto/order/order_type.h"

#include <optional>
#include <string>
#include <vector>

namespace bypto::order {

    using namespace bypto::common::types;
    
    template <OType OT>
    class Order {
        public:
            Order(Symbol sym,Quantity qty,Position pos,OrdSpec<OT> ot) 
                 : m_sym(sym)
                 , m_qty(qty)
                 , m_pos(pos)
                 , m_ot(ot){};

            template<OType T>
            Order(Order<T> o,OrdSpec<OT> ot)
                 : m_sym(o.m_sym)
                 , m_qty(o.m_qty)
                 , m_pos(o.m_pos)
                 , m_ot(ot){};

        Symbol m_sym;
        Quantity m_qty;
        Position m_pos;
        OrdSpec<OT> m_ot;    
    };

    template<OType OrderType>
    std::ostream& operator<<(std::ostream &os,const Order<OrderType> &o) {
        os << "Order {";
        os << "m_sym=" << o.m_sym << ", ";
        os << "m_qty=" << o.m_qty << ", ";
        os << "m_pos=" << o.m_pos << ", ";
        os << o.m_ot << "}";
        return os;
    }
}