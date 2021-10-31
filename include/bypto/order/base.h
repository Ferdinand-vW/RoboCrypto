#pragma once

#include <ostream>

namespace bypto::order {
    
/************************Base types for constructing order types****************/

    enum class Position    { Buy ,Sell };
    enum class TimeInForce { GTC ,IOC, FOK };
    enum class BaseOrQuote { Base,Quote    };

    std::ostream& operator<<(std::ostream &os,const Position &p);
    std::ostream& operator<<(std::ostream &os,const TimeInForce &p);
    std::ostream& operator<<(std::ostream &os,const BaseOrQuote &p);

}