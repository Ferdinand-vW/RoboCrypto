#pragma once

#include "bypto/common/utils.h"
#include "bypto/order_type.h"

#include <optional>
#include <string>
#include <vector>

namespace bypto::order {

    enum Position    { Buy ,Sell     };

    std::ostream& operator<<(std::ostream &os,const Position &p);

    struct Order {
        std::string m_symbol;
        Position m_position;
        order_type::OrderType m_order_type;

    };

    std::ostream& operator<<(std::ostream &os,const Order &o);

    struct GenericOrderInfo {
        std::optional<order_type::TimeInForce> m_time_in_force;
        std::optional<long double> m_quantity;
        std::optional<long double> m_price;
        std::optional<long double> m_stop_price;
        std::optional<order_type::BaseOrQuote> m_base_or_quote;

        GenericOrderInfo(order_type::Market m);
        GenericOrderInfo(order_type::Limit l);
        GenericOrderInfo(order_type::StopLoss sl);
        GenericOrderInfo(order_type::StopLossLimit sll);
        GenericOrderInfo(order_type::TakeProfit tp);
        GenericOrderInfo(order_type::TakeProfitLimit tpl);
        GenericOrderInfo(order_type::LimitMaker lm);
        
        static GenericOrderInfo fromOrderType(order_type::OrderType ot);
    };

    struct GenericOrder {
        std::string m_symbol;
        Position m_position;
        GenericOrderInfo m_order_info;

        GenericOrder(Order order);
    };

}