#include "bypto/order/fill.h"
#include "bypto/order/order_type.h"
#include "bypto/order/types.h"
#include <optional>

namespace bypto::order {
    
    std::ostream& operator<<(std::ostream &os,const Partial &pr) {
        os << "Partial {";
        os << "m_price=" << pr.m_price << ",";
        os << "m_order=" << pr.m_order << ",";
        os << "}";
        return os;
    }

    std::optional<FillResult<Order<OType::Unit>>> fill(Order<OType::Market> m,Price price) {
        Price actual = price;
        if(m.m_ot.m_boq == BaseOrQuote::Quote) {
            actual = 1/price;
            m.m_qty = m.m_qty * actual;
            m.m_pos = opposing(m.m_pos);
        }

        //market order accepts any price
        return orderToFill(actual,m);
    }

    std::optional<FillResult<Order<OType::Unit>>> fill(Order<OType::Limit> l,Price price) {
        if(l.m_pos == Position::Buy && price <= l.m_ot.m_limit) {
            //If price equals or is lower than LIMIT then buy
            return orderToFill(price,l);
        
        } else if (l.m_pos == Position::Sell && price >= l.m_ot.m_limit) {
            //If price equals or is higher than LIMIT then buy
            return orderToFill(price,l);
        
        } else {
            //otherwise don't fill
            return std::nullopt;
        }
    }

    
    std::optional<FillResult<Order<OType::Market>>> fill(Order<OType::StopLoss> sl,Price price) {
        if(sl.m_ot.m_stop >= price) {
        //If price falls and reaches stop price then trigger market order
            Order m(sl,OrdSpec<OType::Market>{BaseOrQuote::Base});
            return orderToFill<OType::StopLoss,OType::Market>(price,sl,m);
        
        } else {
         
            return std::nullopt;
        }   
    }

    std::optional<FillResult<Order<OType::Limit>>> fill(Order<OType::StopLossLimit> sll,Price price) {
        if (sll.m_ot.m_stop >= price) { 
            Order l(sll,OrdSpec<OType::Limit>{sll.m_ot.m_time_in_force,sll.m_ot.m_limit});
            return orderToFill<OType::StopLossLimit,OType::Limit>(price,sll,l);
        
        } else {
            return std::nullopt;
        }
    }

    std::optional<FillResult<Order<OType::Market>>> fill(Order<OType::TakeProfit> sl,Price price) {
        if(sl.m_ot.m_take <= price) {
        //If price falls and reaches stop price then trigger market order
            Order m(sl,OrdSpec<OType::Market>{BaseOrQuote::Base});
            return orderToFill<OType::TakeProfit,OType::Market>(price,sl,m);
        
        } else {
         
            return std::nullopt;
        }   
    }

    std::optional<FillResult<Order<OType::Limit>>> fill(Order<OType::TakeProfitLimit> sll,Price price) {
        if (sll.m_ot.m_take <= price) { 
            Order l(sll,OrdSpec<OType::Limit>{sll.m_ot.m_time_in_force,sll.m_ot.m_limit});
            return orderToFill<OType::TakeProfitLimit,OType::Limit>(price,sll,l);
        
        } else {
            return std::nullopt;
        }
    }

    //TODO
    std::optional<FillResult<Order<OType::Unit>>> fill(OrdSpec<OType::LimitMaker> lm,Price price) {
        return std::nullopt;
    }

}