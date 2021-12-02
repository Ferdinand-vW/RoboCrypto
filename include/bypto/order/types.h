#pragma once

#include "bypto/common/std.h"

#include <ostream>
#include <optional>

namespace bypto::order {
    
/************************Base types for constructing order types****************/

    enum class Position    { Buy ,Sell };
    enum class TimeInForce { GTC ,IOC, FOK };
    enum class BaseOrQuote { Base,Quote    };

    Position opposing(Position p);

    std::optional<Position> pos_from_string(std::string s);
    std::optional<TimeInForce> tif_from_string(std::string s);
    std::optional<BaseOrQuote> boq_from_string(std::string s);

    std::ostream& operator<<(std::ostream &os,const Position &p);
    std::ostream& operator<<(std::ostream &os,const TimeInForce &p);
    std::ostream& operator<<(std::ostream &os,const BaseOrQuote &p);

}