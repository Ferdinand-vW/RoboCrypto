#include "bypto/order_type.h"
#include "bypto/common/types.h"
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

        Partial fillToPartial(FillResult fr) {
        return Partial{fr.m_price,fr.m_qty,fr.m_pos};
    }

    std::ostream& operator<<(std::ostream &os,const FillResult &fr) {
        os << "FillResult {";
        os << "m_price=" << fr.m_price << ",";
        os << "m_qty=" << fr.m_qty << ",";
        os << "m_pos=" << fr.m_pos << ",";
        os << "m_new_order=";
        if(fr.m_new_order) {
            os << fr.m_new_order.value();
        }
        os << "}";
        return os;
    }

    std::ostream& operator<<(std::ostream &os,const Partial &p) {
        os << "Partial {";
        os << "m_price=" << p.m_price << ",";
        os << "m_qty=" << p.m_qty << ",";
        os << "m_pos=" << p.m_pos << "}";
        return os;
    }

    Market::Market(Quantity qty,BaseOrQuote boq) : m_qty(qty),m_boq(boq) {};

    std::optional<FillResult> Market::try_fill(Position pos,Price price) {
        Price actual = price;
        if(m_boq == BaseOrQuote::Quote) {
            actual = 1/price;
        }

        //market order accepts any price
        return FillResult{actual,m_qty,pos,{}};
    }

    std::ostream& operator<<(std::ostream &os,const Market &m) {
        os << "Market {";
        os << "m_qty=" << m.m_qty << ",";
        os << "m_boq=" << m.m_boq      << "}";

        return os;
    }

    Limit::Limit(TimeInForce tif,Quantity qty,Price price) 
                : m_time_in_force(tif)
                , m_qty(qty)
                , m_price(price) {};

    std::optional<FillResult> Limit::try_fill(Position pos,Price price) {
        if(pos == Position::Buy && price <= m_price) {
            //If price equals or is lower than LIMIT then buy
            return FillResult{price,m_qty,pos,{}};
        
        } else if (pos == Position::Sell && price >= m_price) {
            //If price equals or is higher than LIMIT then buy
            return FillResult{price,m_qty,pos,{}};
        
        } else {
            //otherwise don't fill
            return {};
        }
    }

    std::ostream& operator<<(std::ostream &os,const Limit &l) {
        os << "Limit {";
        os << "m_time_in_force=" << l.m_time_in_force << ",";
        os << "m_qty="           << l.m_qty      << ",";
        os << "m_price="         << l.m_price         << "}";

        return os;
    }

    std::optional<FillResult> StopLoss::try_fill(Position pos,Price price) {
        if(m_stop_price >= price) {
        //If price falls and reaches stop price then trigger market order
            return FillResult{price,m_qty,pos,Market(m_qty,BaseOrQuote::Base)};
        
        } else {
         
            return {};
        
        }   
    }

    std::ostream& operator<<(std::ostream &os,const StopLoss &sl) {
        os << "StopLoss {";
        os << "m_qty="        << sl.m_qty   << ",";
        os << "m_stop_price=" << sl.m_stop_price << "}";

        return os;
    }

    std::optional<FillResult> StopLossLimit::try_fill(Position pos,Price price) {
        if (m_stop_price >= price) { 
            return FillResult{price,m_qty,pos,Limit(m_time_in_force,m_qty,m_price)};
        
        } else {
        
            return {};
        
        }
    }

    std::ostream& operator<<(std::ostream &os,const StopLossLimit &sll) {
        os << "StopLossLimit {";
        os << "m_time_in_force=" << sll.m_time_in_force << ",";
        os << "m_qty="           << sll.m_qty      << ",";
        os << "m_price="         << sll.m_price         << ",";
        os << "m_stop_price="    << sll.m_stop_price    << "}";

        return os;
    }

    std::optional<FillResult> TakeProfit::try_fill(Position pos,Price price) {
        if(m_stop_price <= price) {
        //If price increases and reaches stop price then trigger market order
            return FillResult{price,m_qty,pos,Market(m_qty,BaseOrQuote::Base)};
        
        } else {
         
            return {};
        
        }   
    }

    std::ostream& operator<<(std::ostream &os,const TakeProfit &tp) {
        os << "TakeProfit {";
        os << "m_qty="   << tp.m_qty  << ",";
        os << "m_stop_price=" <<tp.m_stop_price << "}";

        return os;
    }

    std::optional<FillResult> TakeProfitLimit::try_fill(Position pos,Price price) {
        if (m_stop_price <= price) { 
            return FillResult{price,m_qty,pos,Limit(m_time_in_force,m_qty,m_price)};
        
        } else {
        
            return {};
        
        }
    }

    std::ostream& operator<<(std::ostream &os,const TakeProfitLimit &tpl) {
        os << "TakeProfitLimit {";
        os << "m_time_in_force=" << tpl.m_time_in_force << ",";
        os << "m_qty="      << tpl.m_qty      << ",";
        os << "m_price="         << tpl.m_price         << ",";
        os << "m_stop_price="    << tpl.m_stop_price    << "}";

        return os;
    }

    std::optional<FillResult> LimitMaker::try_fill(Position pos,Price price) {
        return {};
    }

    std::ostream& operator<<(std::ostream &os,const LimitMaker &lm) {
        os << "LimitMaker {";
        os << "m_qty=" << lm.m_qty << ",";
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