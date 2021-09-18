#pragma once

namespace bypto::order_type {
    enum TimeInForce { GTC ,IOC, FOK };
    enum BaseOrQuote { Base,Quote    };

struct Market {
        long double m_quantity;
        BaseOrQuote m_boq;
    };

    struct Limit {
        TimeInForce m_time_in_force;
        long double m_quantity;
        long double m_price;
    };

    struct StopLoss {
        long double m_quantity;
        long double m_stop_price;
    };

    struct StopLossLimit {
        TimeInForce m_time_in_force;
        long double m_quantity;
        long double m_price;
        long double m_stopPrice;
    };

    struct TakeProfit {
        long double m_quantity;
        long double m_stop_price;
    };

    struct TakeProfitLimit {
        TimeInForce m_time_in_force;
        long double m_quantity;
        long double m_price;
        long double m_stop_price;
    };

    struct LimitMaker {
        long double m_quantity;
        long double m_price;
    };

}

