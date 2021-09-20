#pragma once

#include <ostream>
#include <variant>

namespace bypto::order_type {
    enum TimeInForce { GTC ,IOC, FOK };
    enum BaseOrQuote { Base,Quote    };

    std::ostream& operator<<(std::ostream &os,const TimeInForce &p);
    std::ostream& operator<<(std::ostream &os,const BaseOrQuote &p);

    struct Market {
        long double m_quantity;
        BaseOrQuote m_boq;
    };

    std::ostream& operator<<(std::ostream &os,const Market &m);

    struct Limit {
        TimeInForce m_time_in_force;
        long double m_quantity;
        long double m_price;
    };

    std::ostream& operator<<(std::ostream &os,const Limit &l);

    struct StopLoss {
        long double m_quantity;
        long double m_stop_price;
    };

    std::ostream& operator<<(std::ostream &os,const StopLoss &sl);

    struct StopLossLimit {
        TimeInForce m_time_in_force;
        long double m_quantity;
        long double m_price;
        long double m_stop_price;
    };

    std::ostream& operator<<(std::ostream &os,const StopLossLimit &sll);

    struct TakeProfit {
        long double m_quantity;
        long double m_stop_price;
    };

    std::ostream& operator<<(std::ostream &os,const TakeProfit &tp);

    struct TakeProfitLimit {
        TimeInForce m_time_in_force;
        long double m_quantity;
        long double m_price;
        long double m_stop_price;
    };

    std::ostream& operator<<(std::ostream &os,const TakeProfitLimit &tpl);

    struct LimitMaker {
        long double m_quantity;
        long double m_price;
    };

    std::ostream& operator<<(std::ostream &os,const LimitMaker &lm);

    typedef std::variant<Market,Limit,StopLoss,StopLossLimit,TakeProfit,TakeProfitLimit,LimitMaker> OrderType;

    std::ostream& operator<<(std::ostream &os,const OrderType &ot);

}

