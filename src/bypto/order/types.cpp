#include "bypto/order/types.h"
#include "bypto/common/std.h"

namespace bypto::order {

Position opposing(Position p) {
    switch (p) {
    case Position::Sell:
        return Position::Buy;
        break;
    case Position::Buy:
        return Position::Sell;
        break;
    }
}

std::optional<Position> pos_from_string(std::string s) {
    if (s == "Buy") {
        return Position::Buy;
    } else if (s == "Sell") {
        return Position::Sell;
    } else {
        return std::nullopt;
    }
}

std::optional<TimeInForce> tif_from_string(std::string s) {
    if (s == "GTC") {
        return TimeInForce::GTC;
    } else if (s == "IOC") {
        return TimeInForce::IOC;
    } else if (s == "FOK") {
        return TimeInForce::FOK;
    } else {
        return std::nullopt;
    }
}

std::optional<BaseOrQuote> boq_from_string(std::string s) {
    if (s == "Base") {
        return BaseOrQuote::Base;
    } else if (s == "Quote") {
        return BaseOrQuote::Quote;
    } else {
        return std::nullopt;
    }
}

std::ostream &operator<<(std::ostream &os, const Position &p) {
    switch (p) {
    case Position::Buy:
        os << "Buy";
        break;
    case Position::Sell:
        os << "Sell";
        break;
    }

    return os;
}

std::ostream &operator<<(std::ostream &os, const TimeInForce &tif) {
    switch (tif) {
    case TimeInForce::GTC:
        os << "GTC";
        break;
    case TimeInForce::IOC:
        os << "IOC";
        break;
    case TimeInForce::FOK:
        os << "FOK";
        break;
    }

    return os;
}

std::ostream &operator<<(std::ostream &os, const BaseOrQuote &boq) {
    switch (boq) {
    case BaseOrQuote::Base:
        os << "Base";
        break;
    case BaseOrQuote::Quote:
        os << "Quote";
        break;
    }

    return os;
}
} // namespace bypto::order