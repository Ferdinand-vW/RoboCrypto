#include "bypto/order/fill.h"
#include "bypto/order/order_type.h"
#include "bypto/order/types.h"
#include <optional>

namespace bypto::order {
    
    std::ostream& operator<<(std::ostream &os,const Partial &fr) {
        os << "Partial {";
        os << "m_price=" << fr.m_price << ",";
        os << "m_qty=" << fr.m_qty << ",";
        os << "m_pos=" << fr.m_pos << ",";
        os << "}";
        return os;
    }

    std::optional<FillResult<Order<Unit>>> fill(Order<Market> m,Price price) {
        Price actual = price;
        if(m.m_ot.m_boq == BaseOrQuote::Quote) {
            actual = 1/price;
        }

        //market order accepts any price
        return orderToFill(actual,m);
    }

    std::optional<FillResult<Order<Unit>>> fill(Order<Limit> l,Price price) {
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

    
    std::optional<FillResult<Order<Market>>> fill(Order<StopLoss> sl,Price price) {
        if(sl.m_ot.m_stop >= price) {
        //If price falls and reaches stop price then trigger market order
            Order m(sl,Market{BaseOrQuote::Base});
            return orderToFill<StopLoss,Market>(price,sl,m);
        
        } else {
         
            return std::nullopt;
        }   
    }

    std::optional<FillResult<Order<Limit>>> fill(Order<StopLossLimit> sll,Price price) {
        if (sll.m_ot.m_stop >= price) { 
            Order l(sll,Limit{sll.m_ot.m_time_in_force,sll.m_ot.m_limit});
            return orderToFill<StopLossLimit,Limit>(price,sll,l);
        
        } else {
            return std::nullopt;
        }
    }

    std::optional<FillResult<Order<Market>>> fill(Order<TakeProfit> sl,Price price) {
        if(sl.m_ot.m_take <= price) {
        //If price falls and reaches stop price then trigger market order
            Order m(sl,Market{BaseOrQuote::Base});
            return orderToFill<TakeProfit,Market>(price,sl,m);
        
        } else {
         
            return std::nullopt;
        }   
    }

    std::optional<FillResult<Order<Limit>>> fill(Order<TakeProfitLimit> sll,Price price) {
        if (sll.m_ot.m_take <= price) { 
            Order l(sll,Limit{sll.m_ot.m_time_in_force,sll.m_ot.m_limit});
            return orderToFill<TakeProfitLimit,Limit>(price,sll,l);
        
        } else {
            return std::nullopt;
        }
    }

    //TODO
    std::optional<FillResult<Order<Unit>>> fill(LimitMaker lm,Price price) {
        return std::nullopt;
    }

}