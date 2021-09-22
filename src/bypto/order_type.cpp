#include "bypto/order_type.h"
#include "bypto/common/utils.h"

#include <ostream>

namespace bypto::order_type {

    std::ostream& operator<<(std::ostream &os,const Position &p) {
        switch(p) {
            case Buy: os << "Buy"; break;
            case Sell: os << "Sell"; break;
        }

        return os;
    }

    std::ostream& operator<<(std::ostream &os,const TimeInForce &tif) {
        switch(tif) {
            case GTC: os << "GTC"; break;
            case IOC: os << "IOC"; break;
            case FOK: os << "FOK"; break;
        }

        return os;
    }

    std::ostream& operator<<(std::ostream &os,const BaseOrQuote &boq) {
        switch(boq) {
            case Base: os  << "Base"; break;
            case Quote: os << "Quote"; break;
        }

        return os;
    }

    std::optional<std::tuple<Price,Quantity>> Market::try_fill(Price price) {
        Price actual = price;
        if(m_boq == BaseOrQuote::Quote) {
            actual = 1/price;
        }

        //market order accepts any price
        return std::make_tuple(actual,m_quantity);
    }

    std::ostream& operator<<(std::ostream &os,const Market &m) {
        os << "Market {";
        os << "m_quantity=" << m.m_quantity << ",";
        os << "m_boq="      << m.m_boq      << "}";

        return os;
    }

    std::optional<std::tuple<Price,Quantity>> Limit::try_fill(Position ord_pos,Price price) {
        if(ord_pos == Position::Buy && price <= m_price) {
            //If price equals or is lower than LIMIT then buy
            return std::make_tuple(price,m_quantity);
        
        } else if (ord_pos == Position::Sell && price >= m_price) {
            //If price equals or is higher than LIMIT then buy
            return std::make_tuple(price,m_quantity);
        
        } else {
            //otherwise don't fill
            return {};
        
        }
    }

    std::ostream& operator<<(std::ostream &os,const Limit &l) {
        os << "Limit {";
        os << "m_time_in_force=" << l.m_time_in_force << ",";
        os << "m_quantity="      << l.m_quantity      << ",";
        os << "m_price="         << l.m_price         << "}";

        return os;
    }

    std::optional<std::tuple<Price,Quantity>> StopLoss::try_fill(Price price) {
        if(m_stop_price >= price) {
        //If price falls and reaches stop price then trigger market order
            return std::make_tuple(price,m_quantity);
        
        } else {
         
            return {};
        
        }   
    }

    std::ostream& operator<<(std::ostream &os,const StopLoss &sl) {
        os << "StopLoss {";
        os << "m_quantity="   << sl.m_quantity   << ",";
        os << "m_stop_price=" << sl.m_stop_price << "}";

        return os;
    }

    std::optional<
        std::tuple<Price,Quantity,std::optional<Limit>>
        > StopLossLimit::try_fill(Price price) {
        if (m_stop_price >= price) { 
            return std::make_tuple(price,m_quantity,Limit{m_time_in_force,m_quantity,m_price});
        
        } else {
        
            return {};
        
        }
    }

    std::ostream& operator<<(std::ostream &os,const StopLossLimit &sll) {
        os << "StopLossLimit {";
        os << "m_time_in_force=" << sll.m_time_in_force << ",";
        os << "m_quantity="      << sll.m_quantity      << ",";
        os << "m_price="         << sll.m_price         << ",";
        os << "m_stop_price="    << sll.m_stop_price    << "}";

        return os;
    }

    std::optional<std::tuple<Price,Quantity>> TakeProfit::try_fill(Price price) {
        if(m_stop_price <= price) {
        //If price increases and reaches stop price then trigger market order
            return std::make_tuple(price,m_quantity);
        
        } else {
         
            return {};
        
        }   
    }

    std::ostream& operator<<(std::ostream &os,const TakeProfit &tp) {
        os << "TakeProfit {";
        os << "m_quantity="   << tp.m_quantity  << ",";
        os << "m_stop_price=" <<tp.m_stop_price << "}";

        return os;
    }

    std::optional<
        std::tuple<Price,Quantity,std::optional<Limit>>
        > TakeProfitLimit::try_fill(Price price) {
        if (m_stop_price <= price) { 
            return std::make_tuple(price,m_quantity,Limit{m_time_in_force,m_quantity,m_price});
        
        } else {
        
            return {};
        
        }
    }

    std::ostream& operator<<(std::ostream &os,const TakeProfitLimit &tpl) {
        os << "TakeProfitLimit {";
        os << "m_time_in_force=" << tpl.m_time_in_force << ",";
        os << "m_quantity="      << tpl.m_quantity      << ",";
        os << "m_price="         << tpl.m_price         << ",";
        os << "m_stop_price="    << tpl.m_stop_price    << "}";

        return os;
    }

    std::ostream& operator<<(std::ostream &os,const LimitMaker &lm) {
        os << "LimitMaker {";
        os << "m_quantity=" << lm.m_quantity << ",";
        os << "m_price="    << lm.m_price << "}";

        return os;
    }

    std::ostream& operator<<(std::ostream &os,const OrderType &ot) {
        std::visit(common::utils::overload{
            [&os](Market m) { os << m;},
            [&os](Limit l) { os << l; },
            [&os](StopLoss sl) { os << sl;},
            [&os](StopLossLimit sll) { os << sll; },
            [&os](TakeProfit tp) { os << tp; },
            [&os](TakeProfitLimit tpl) { os << tpl;},
            [&os](LimitMaker lm) { os << lm; }
        },ot);

        return os;
    }

}