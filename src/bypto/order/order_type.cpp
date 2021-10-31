#include "bypto/order/order_type.h"
#include "bypto/common/types.h"
#include "bypto/common/utils.h"

#include <ostream>

namespace bypto::order {
    
    std::ostream& operator<<(std::ostream &os,const Market &m) {
        os << "Market {";
        os << "m_boq=" << m.m_boq      << "}";

        return os;
    }


    std::ostream& operator<<(std::ostream &os,const Limit &l) {
        os << "Limit {";
        os << "m_time_in_force=" << l.m_time_in_force << ",";
        os << "m_limit="         << l.m_limit         << "}";

        return os;
    }

    std::ostream& operator<<(std::ostream &os,const StopLoss &sl) {
        os << "StopLoss {";
        os << "m_stop=" << sl.m_stop << "}";

        return os;
    }

    std::ostream& operator<<(std::ostream &os,const StopLossLimit &sll) {
        os << "StopLossLimit {";
        os << "m_time_in_force=" << sll.m_time_in_force << ",";
        os << "m_limit="         << sll.m_limit         << ",";
        os << "m_stop="          << sll.m_stop    << "}";

        return os;
    }

    std::ostream& operator<<(std::ostream &os,const TakeProfit &tp) {
        os << "TakeProfit {";
        os << "m_take=" <<tp.m_take << "}";

        return os;
    }

    std::ostream& operator<<(std::ostream &os,const TakeProfitLimit &tpl) {
        os << "TakeProfitLimit {";
        os << "m_time_in_force=" << tpl.m_time_in_force << ",";
        os << "m_limit="         << tpl.m_limit         << ",";
        os << "m_take="          << tpl.m_take    << "}";

        return os;
    }

    std::ostream& operator<<(std::ostream &os,const LimitMaker &lm) {
        os << "LimitMaker {";
        os << "m_limit="    << lm.m_limit << "}";

        return os;
    }
}