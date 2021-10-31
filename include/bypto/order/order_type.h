#pragma once

#include "bypto/common/types.h"
#include "bypto/order/types.h"

#include <ostream>
#include <variant>

namespace bypto::order {
    using namespace bypto::common::types;

/****************************Order contract******************************/


/*********************Concrete Order Types**********************/
    struct Market {
        BaseOrQuote m_boq;
    };

    std::ostream& operator<<(std::ostream &os,const Market &m);

    struct Limit {
        TimeInForce m_time_in_force;
        Price m_limit;
    };

    std::ostream& operator<<(std::ostream &os,const Limit &l);

    struct StopLoss {
        Price m_stop;
    };

    std::ostream& operator<<(std::ostream &os,const StopLoss &sl);

    struct StopLossLimit {
        TimeInForce m_time_in_force;
        Price m_limit;
        Price m_stop;
    };

    std::ostream& operator<<(std::ostream &os,const StopLossLimit &sll);

    struct TakeProfit {
        Price m_take;
    };

    std::ostream& operator<<(std::ostream &os,const TakeProfit &tp);

    struct TakeProfitLimit {
        TimeInForce m_time_in_force;
        Price m_limit;
        Price m_take;
    };

    std::ostream& operator<<(std::ostream &os,const TakeProfitLimit &tpl);

    struct LimitMaker {
        Price m_limit;
    };

    std::ostream& operator<<(std::ostream &os,const LimitMaker &lm);
}

