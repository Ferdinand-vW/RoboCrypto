#pragma once

#include "bypto/common/utils.h"
#include "bypto/common/types.h"
#include "bypto/order/base.h"
#include "bypto/order/order_info.h"

#include <optional>
#include <string>
#include <vector>

namespace bypto::order {

    using namespace bypto::common::types;

    template <typename OrderType
             ,std::enable_if_t<std::is_base_of<OrderInfo,OrderType>::value>* = nullptr> //enforce OrderType inherits Orderable
    class Order {
        OrderType m_ot;

        public:
            Order(OrderType ot) : m_ot(ot){};

            OrderType get_order_type() const {
                return m_ot;
            }

            Symbol get_symbol() const {
                return m_ot.get_symbol();
            }

            Quantity get_quantity() const {
                return m_ot.get_quantity();
            }


            Position get_position() const {
                return m_ot.get_position();
            }

            BaseOrQuote get_base_or_quote() const {
                return m_ot.get_base_or_quote();
            }
    };

    template<typename OrderType>
    std::ostream& operator<<(std::ostream &os,const Order<OrderType> &o) {
        os << o.get_order_type();
        return os;
    }

    // struct GenericOrderInfo {
    //     std::optional<order_type::TimeInForce> m_time_in_force;
    //     std::optional<Quantity> m_qty;
    //     std::optional<Price> m_price;
    //     std::optional<Price> m_stop_price;
    //     std::optional<order_type::BaseOrQuote> m_base_or_quote;

    //     GenericOrderInfo(order_type::Market m);
    //     GenericOrderInfo(order_type::Limit l);
    //     GenericOrderInfo(order_type::StopLoss sl);
    //     GenericOrderInfo(order_type::StopLossLimit sll);
    //     GenericOrderInfo(order_type::TakeProfit tp);
    //     GenericOrderInfo(order_type::TakeProfitLimit tpl);
    //     GenericOrderInfo(order_type::LimitMaker lm);
        
    //     static GenericOrderInfo fromOrderType(order_type::OrderType ot);
    // };

    // struct GenericOrder {
    //     Symbol m_symbol;
    //     order_type::Position m_pos;
    //     GenericOrderInfo m_order_info;

    //     template <typename OrderType>
    //     GenericOrder(Order<OrderType> order);
    // };

}