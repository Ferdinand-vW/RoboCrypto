#pragma once

#include "bypto/common/types.h"
#include "bypto/common/std.h"
#include "bypto/order/types.h"

#include <ostream>
#include <variant>

namespace bypto::order {
    using namespace bypto::common::types;

/****************************Order contract******************************/

enum class OType { Unit, Market, Limit, StopLoss, StopLossLimit, TakeProfit, TakeProfitLimit, LimitMaker };

std::ostream& operator<< (std::ostream &os,const OType &ot);

std::optional<OType> otype_from_string(std::string s);

template <OType OT>
class OrdSpec {};

/*********************Concrete Order Types**********************/
    template<>
    class OrdSpec<OType::Market> {
        public:
            BaseOrQuote m_boq;

            std::string get_name() const;
    };

    std::ostream& operator<<(std::ostream &os,const OrdSpec<OType::Market> &m);

    template<>
    class OrdSpec<OType::Limit> {
        public:
            TimeInForce m_time_in_force;
            Price m_limit;

            std::string get_name() const;
    };

    std::ostream& operator<<(std::ostream &os,const OrdSpec<OType::Limit> &l);

    template<>
    class OrdSpec<OType::StopLoss> {
        public:
            Price m_stop;

            std::string get_name() const;
    };

    std::ostream& operator<<(std::ostream &os,const OrdSpec<OType::StopLoss> &sl);

    template<>
    class OrdSpec<OType::StopLossLimit> {
        public:
            TimeInForce m_time_in_force;
            Price m_limit;
            Price m_stop;

            std::string get_name() const;
    };

    std::ostream& operator<<(std::ostream &os,const OrdSpec<OType::StopLossLimit> &sll);

    template<>
    class OrdSpec<OType::TakeProfit> {
        public:
            Price m_take;
            
            std::string get_name() const;
    };

    std::ostream& operator<<(std::ostream &os,const OrdSpec<OType::TakeProfit> &tp);

    template<>
    class OrdSpec<OType::TakeProfitLimit> {
        public:
            TimeInForce m_time_in_force;
            Price m_limit;
            Price m_take;

            std::string get_name() const;
    };

    std::ostream& operator<<(std::ostream &os,const OrdSpec<OType::TakeProfitLimit> &tpl);

    template<>
    class OrdSpec<OType::LimitMaker> {
        public:
            Price m_limit;

            std::string get_name() const;
    };

    std::ostream& operator<<(std::ostream &os,const OrdSpec<OType::LimitMaker> &lm);
}

