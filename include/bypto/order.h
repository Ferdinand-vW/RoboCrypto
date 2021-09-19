#pragma once

#include "bypto/common/utils.h"
#include "bypto/order_type.h"

#include <optional>
#include <string>
#include <vector>

namespace bypto::order {

    enum Position    { Buy ,Sell     };

    std::ostream& operator<<(std::ostream &os,const Position &p);

    template <class OrderType>
    struct Order {
        std::string m_symbol;
        Position m_position;
        OrderType m_order_type;
    };

    template <class T>
    std::ostream& operator<<(std::ostream &os,const Order<T> &o) {
        os << "Order {";
        os << "m_symbol="     << o.m_symbol     << ",";
        os << "m_position="   << o.m_position   << ",";
        os << "m_order_type=" << o.m_order_type << "}";
        return os;
    }

    struct GenericOrderInfo {
        std::optional<order_type::TimeInForce> m_time_in_force;
        std::optional<long double> m_quantity;
        std::optional<long double> m_price;
        std::optional<long double> m_stop_price;
        std::optional<order_type::BaseOrQuote> m_base_or_quote;

        static GenericOrderInfo from_order_type(order_type::Market m);
        static GenericOrderInfo from_order_type(order_type::Limit m);
        static GenericOrderInfo from_order_type(order_type::StopLoss m);
        static GenericOrderInfo from_order_type(order_type::StopLossLimit m);
        static GenericOrderInfo from_order_type(order_type::TakeProfit m);
        static GenericOrderInfo from_order_type(order_type::TakeProfitLimit m);
        static GenericOrderInfo from_order_type(order_type::LimitMaker m);
    };

    

    struct GenericOrder {
        std::string m_symbol;
        Position m_position;
        GenericOrderInfo m_order_info;
    };

    template <class T>
    GenericOrder orderToGeneric(Order<T> order) {
        return GenericOrder{order.m_symbol,order.m_position
                        ,order.m_order_type.to_generic_order_info()
                        };
    }


    

}