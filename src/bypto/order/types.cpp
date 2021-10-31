#include "bypto/order/types.h"

namespace bypto::order {
    std::ostream& operator<<(std::ostream &os,const Position &p) {
        switch(p) {
            case Position::Buy: os << "Buy"; break;
            case Position::Sell: os << "Sell"; break;
        }

        return os;
    }

    std::ostream& operator<<(std::ostream &os,const TimeInForce &tif) {
        switch(tif) {
            case TimeInForce::GTC: os << "GTC"; break;
            case TimeInForce::IOC: os << "IOC"; break;
            case TimeInForce::FOK: os << "FOK"; break;
        }

        return os;
    }

    std::ostream& operator<<(std::ostream &os,const BaseOrQuote &boq) {
        switch(boq) {
            case BaseOrQuote::Base: os  << "Base"; break;
            case BaseOrQuote::Quote: os << "Quote"; break;
        }

        return os;
    }
}