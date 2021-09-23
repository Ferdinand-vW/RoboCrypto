#pragma once

#include "bypto/common/utils.h"
#include "bypto/common/types.h"
#include "bypto/order_type.h"

#include <optional>
#include <string>
#include <vector>

namespace bypto::order {

    using namespace bypto::common::types;

    class Order {

        public:
            Order(Symbol symbol,order_type::Position pos,order_type::OrderType ot);

            Symbol m_symbol;
            order_type::Position m_pos;
            order_type::OrderType m_order_type;

            std::optional<order_type::Partial> try_fill(Symbol symbol,Price price);
    };

    class OrderResult {
        Order m_order;
        std::vector<order_type::Partial> m_partials;

        public:
            OrderResult(Order order);

        bool is_partial();
        Quantity get_quantity();
        long double get_value();

    };

    std::ostream& operator<<(std::ostream &os,const Order &o);

    struct GenericOrderInfo {
        std::optional<order_type::TimeInForce> m_time_in_force;
        std::optional<Quantity> m_qty;
        std::optional<Price> m_price;
        std::optional<Price> m_stop_price;
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
        Symbol m_symbol;
        order_type::Position m_pos;
        GenericOrderInfo m_order_info;

        GenericOrder(Order order);
    };

}