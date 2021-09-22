#pragma once

#include "bypto/common/types.h"

#include <ostream>
#include <variant>

namespace bypto::order_type {
    using namespace bypto::common::types;

    enum Position    { Buy ,Sell };
    enum TimeInForce { GTC ,IOC, FOK };
    enum BaseOrQuote { Base,Quote    };

    std::ostream& operator<<(std::ostream &os,const Position &p);
    std::ostream& operator<<(std::ostream &os,const TimeInForce &p);
    std::ostream& operator<<(std::ostream &os,const BaseOrQuote &p);

    struct Market {
        Quantity m_quantity;
        BaseOrQuote m_boq;

        std::optional<std::tuple<Price,Quantity>> try_fill(Price price);
    };

    std::ostream& operator<<(std::ostream &os,const Market &m);

    struct Limit {
        TimeInForce m_time_in_force;
        Quantity m_quantity;
        Price m_price;

        std::optional<std::tuple<Price,Quantity>> try_fill(Position pos,Price price);
    };

    std::ostream& operator<<(std::ostream &os,const Limit &l);

    struct StopLoss {
        Quantity m_quantity;
        Price m_stop_price;

        std::optional<std::tuple<Price,Quantity>> try_fill(Price price);
    };

    std::ostream& operator<<(std::ostream &os,const StopLoss &sl);

    struct StopLossLimit {
        TimeInForce m_time_in_force;
        Quantity m_quantity;
        Price m_price;
        Price m_stop_price;

        std::optional<
            std::tuple<Price,Quantity,std::optional<Limit>>
            > try_fill(Price price);
    };

    std::ostream& operator<<(std::ostream &os,const StopLossLimit &sll);

    struct TakeProfit {
        Quantity m_quantity;
        Price m_stop_price;

        std::optional<std::tuple<Price,Quantity>> try_fill(Price price);
    };

    std::ostream& operator<<(std::ostream &os,const TakeProfit &tp);

    struct TakeProfitLimit {
        TimeInForce m_time_in_force;
        Quantity m_quantity;
        Price m_price;
        Price m_stop_price;

        std::optional<
            std::tuple<Price,Quantity,std::optional<Limit>>
            > try_fill(Price price);
    };

    std::ostream& operator<<(std::ostream &os,const TakeProfitLimit &tpl);

    struct LimitMaker {
        Quantity m_quantity;
        Price m_price;
    };

    std::ostream& operator<<(std::ostream &os,const LimitMaker &lm);

    typedef std::variant<Market,Limit,StopLoss,StopLossLimit,TakeProfit,TakeProfitLimit,LimitMaker> OrderType;

    std::ostream& operator<<(std::ostream &os,const OrderType &ot);

}

