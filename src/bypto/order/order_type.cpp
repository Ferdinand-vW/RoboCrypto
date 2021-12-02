#include "bypto/order/order_type.h"
#include "bypto/common/types.h"
#include "bypto/common/utils.h"
#include "bypto/common/std.h"

#include <ostream>

namespace bypto::order {
    
    std::optional<OType> otype_from_string(std::string s) {
        if(s == "Unit") {
            return OType::Unit;
        }
        else if(s=="Market") {
            return OType::Market;
        }
        else if(s=="Limit") {
            return OType::Limit;
        }
        else if(s=="StopLoss") {
            return OType::StopLoss;
        }
        else if(s=="StopLossLimit") {
            return OType::StopLossLimit;
        }
        else if(s=="TakeProfit") {
            return OType::TakeProfit;
        }
        else if(s=="TakeProfitLimit") {
            return OType::TakeProfitLimit;
        }
        else if(s=="LimitMaker") {
            return OType::LimitMaker;
        }
        else {
            return std::nullopt;
        }
    }

    std::ostream& operator<< (std::ostream &os,const OType &ot) {
        switch(ot) {
            case OType::Unit:            os << "Unit"; break;
            case OType::Market:          os << "Market"; break;
            case OType::Limit:           os << "Limit"; break;
            case OType::StopLoss:        os << "StopLoss"; break;
            case OType::StopLossLimit:   os << "StopLossLimit"; break;
            case OType::TakeProfit:      os << "TakeProfit"; break;
            case OType::TakeProfitLimit: os << "TakeProfitLimit"; break;
            case OType::LimitMaker:      os << "LimitMaker"; break;
        }

        return os;
    }

    std::string OrdSpec<OType::Market>::get_name() const { return "Market"; }

    std::ostream& operator<<(std::ostream &os,const OrdSpec<OType::Market> &m) {
        os << "Market {";
        os << "m_boq=" << m.m_boq      << "}";

        return os;
    }

    std::string OrdSpec<OType::Limit>::get_name() const { return "Limit"; }

    std::ostream& operator<<(std::ostream &os,const OrdSpec<OType::Limit> &l) {
        os << "Limit {";
        os << "m_time_in_force=" << l.m_time_in_force << ",";
        os << "m_limit="         << l.m_limit         << "}";

        return os;
    }

    std::string OrdSpec<OType::StopLoss>::get_name() const { return "StopLoss"; }

    std::ostream& operator<<(std::ostream &os,const OrdSpec<OType::StopLoss> &sl) {
        os << "StopLoss {";
        os << "m_stop=" << sl.m_stop << "}";

        return os;
    }

    std::string OrdSpec<OType::StopLossLimit>::get_name() const { return "StopLossLimit"; }

    std::ostream& operator<<(std::ostream &os,const OrdSpec<OType::StopLossLimit> &sll) {
        os << "StopLossLimit {";
        os << "m_time_in_force=" << sll.m_time_in_force << ",";
        os << "m_limit="         << sll.m_limit         << ",";
        os << "m_stop="          << sll.m_stop    << "}";

        return os;
    }

    std::string OrdSpec<OType::TakeProfit>::get_name() const { return "TakeProfit"; }

    std::ostream& operator<<(std::ostream &os,const OrdSpec<OType::TakeProfit> &tp) {
        os << "TakeProfit {";
        os << "m_take=" <<tp.m_take << "}";

        return os;
    }

    std::string OrdSpec<OType::TakeProfitLimit>::get_name() const { return "TakeProfitLimit"; }

    std::ostream& operator<<(std::ostream &os,const OrdSpec<OType::TakeProfitLimit> &tpl) {
        os << "TakeProfitLimit {";
        os << "m_time_in_force=" << tpl.m_time_in_force << ",";
        os << "m_limit="         << tpl.m_limit         << ",";
        os << "m_take="          << tpl.m_take    << "}";

        return os;
    }

    std::string OrdSpec<OType::LimitMaker>::get_name() const { return "LimitMaker"; }

    std::ostream& operator<<(std::ostream &os,const OrdSpec<OType::LimitMaker> &lm) {
        os << "LimitMaker {";
        os << "m_limit="    << lm.m_limit << "}";

        return os;
    }
}