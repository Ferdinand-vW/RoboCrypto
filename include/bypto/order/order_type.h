#pragma once

#include "bypto/common/types.h"
#include "bypto/order/types.h"

#include <ostream>
#include <variant>

namespace bypto::order {
    using namespace bypto::common::types;

/****************************Order contract******************************/


/*********************Concrete Order Types**********************/
    class Market {
        public:
            BaseOrQuote m_boq;

            std::string get_name() const;
    };

    std::ostream& operator<<(std::ostream &os,const Market &m);

    class Limit {
        public:
            TimeInForce m_time_in_force;
            Price m_limit;

            std::string get_name() const;
    };

    std::ostream& operator<<(std::ostream &os,const Limit &l);

    class StopLoss {
        public:
            Price m_stop;

            std::string get_name() const;
    };

    std::ostream& operator<<(std::ostream &os,const StopLoss &sl);

    class StopLossLimit {
        public:
            TimeInForce m_time_in_force;
            Price m_limit;
            Price m_stop;

            std::string get_name() const;
    };

    std::ostream& operator<<(std::ostream &os,const StopLossLimit &sll);

    class TakeProfit {
        public:
            Price m_take;
            
            std::string get_name() const;
    };

    std::ostream& operator<<(std::ostream &os,const TakeProfit &tp);

    struct TakeProfitLimit {
        public:
            TimeInForce m_time_in_force;
            Price m_limit;
            Price m_take;

            std::string get_name() const;
    };

    std::ostream& operator<<(std::ostream &os,const TakeProfitLimit &tpl);

    class LimitMaker {
        public:
            Price m_limit;

            std::string get_name() const;
    };

    std::ostream& operator<<(std::ostream &os,const LimitMaker &lm);
}

