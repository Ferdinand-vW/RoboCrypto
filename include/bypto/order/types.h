#pragma once

#include "bypto/common/types.h"
#include "bypto/order/base.h"
#include "bypto/order/order_info.h"

#include <ostream>
#include <variant>

namespace bypto::order {
    using namespace bypto::common::types;

/****************************Order contract******************************/


/*********************Concrete Order Types**********************/

    class Market : OrderInfo {
        public:
            Market(Symbol sym,Quantity qty,Position pos,BaseOrQuote boq);

            Symbol get_symbol() const;
            Quantity get_quantity() const;
            Position get_position() const;
            BaseOrQuote get_base_or_quote() const;
            std::optional<FillResult> try_fill(Price price) const;

        private:
            Symbol m_sym;
            Quantity m_qty;
            Position m_pos;
            BaseOrQuote m_boq;
    };

    std::ostream& operator<<(std::ostream &os,const Market &m);

    class Limit : OrderInfo {
        public:
            Limit(Symbol sym,Quantity qty,Position pos,TimeInForce tif,Price limit);

            Symbol get_symbol() const;
            Quantity get_quantity() const;
            Position get_position() const;
            BaseOrQuote get_base_or_quote() const;
            TimeInForce get_time_in_force() const;
            Price get_limit() const;
            std::optional<FillResult> try_fill(Price price) const;

        private:
            Symbol m_sym;
            Quantity m_qty;
            Position m_pos;
            BaseOrQuote m_boq;
            TimeInForce m_time_in_force;
            Price m_limit;
    };

    std::ostream& operator<<(std::ostream &os,const Limit &l);

    struct StopLoss : OrderInfo {
        public:
            StopLoss(Symbol sym,Quantity qty,Position pos,Price stop);

            Symbol get_symbol() const;
            Quantity get_quantity() const;
            Position get_position() const;
            BaseOrQuote get_base_or_quote() const;
            Price get_stop() const;
            std::optional<FillResult> try_fill(Price price) const;

        private:
            Symbol m_sym;
            Quantity m_qty;
            Position m_pos;
            BaseOrQuote m_boq;
            Price m_stop;
    };

    std::ostream& operator<<(std::ostream &os,const StopLoss &sl);

    struct StopLossLimit : OrderInfo {
        public:
            StopLossLimit(Symbol sym,Quantity qty,Position pos
                         ,BaseOrQuote boq,TimeInForce tif,Price limit
                         ,Price stop);
            

            Symbol get_symbol() const;
            Quantity get_quantity() const;
            Position get_position() const;
            BaseOrQuote get_base_or_quote() const;
            TimeInForce get_time_in_force() const;
            Price get_limit() const;
            Price get_stop() const;
            std::optional<FillResult> try_fill(Price price) const;

        private:
            Symbol m_sym;
            Quantity m_qty;
            Position m_pos;
            BaseOrQuote m_boq;
            TimeInForce m_time_in_force;
            Price m_limit;
            Price m_stop;
    };

    std::ostream& operator<<(std::ostream &os,const StopLossLimit &sll);

    struct TakeProfit : OrderInfo {
        public:
            TakeProfit(Symbol sym,Quantity qty,Position pos,BaseOrQuote boq,Price take);
            
            Symbol get_symbol() const;
            Quantity get_quantity() const;
            Position get_position() const;
            BaseOrQuote get_base_or_quote() const;
            Price get_take() const;
            std::optional<FillResult> try_fill(Price price) const;

        private:
            Symbol m_sym;
            Quantity m_qty;
            Position m_pos;
            BaseOrQuote m_boq;
            Price m_take;
    };

    std::ostream& operator<<(std::ostream &os,const TakeProfit &tp);

    struct TakeProfitLimit : OrderInfo {
        public:
            TakeProfitLimit(Symbol sym,Quantity qty,Position pos
                           ,BaseOrQuote boq,TimeInForce tif,Price limit
                           ,Price take);

            Symbol get_symbol() const;
            Quantity get_quantity() const;
            Position get_position() const;
            BaseOrQuote get_base_or_quote() const;
            TimeInForce get_time_in_force() const;
            Price get_limit() const;
            Price get_take() const;
            std::optional<FillResult> try_fill(Price price) const;

        private:
            Symbol m_sym;
            Quantity m_qty;
            Position m_pos;
            BaseOrQuote m_boq;
            TimeInForce m_time_in_force;
            Price m_limit;
            Price m_take;
    };

    std::ostream& operator<<(std::ostream &os,const TakeProfitLimit &tpl);

    struct LimitMaker : OrderInfo {
        public:
            LimitMaker(Symbol sym,Quantity qty,Position pos,BaseOrQuote boq,Price limit);

            Symbol get_symbol() const;
            Quantity get_quantity() const;
            Position get_position() const;
            BaseOrQuote get_base_or_quote() const;
            Price get_limit() const;
            std::optional<FillResult> try_fill(Price price) const;

        private:
            Symbol m_sym;
            Quantity m_qty;
            Position m_pos;
            BaseOrQuote m_boq;
            Price m_limit;
    };

    std::ostream& operator<<(std::ostream &os,const LimitMaker &lm);

    typedef std::variant<Market,Limit,StopLoss,StopLossLimit,TakeProfit,TakeProfitLimit,LimitMaker> OrderType;

    std::ostream& operator<<(std::ostream &os,const OrderType &ot);

/****************************Order fill types******************************/

    struct Partial {
        Price m_price;
        Symbol m_symbol;
        Quantity m_qty;
        Position m_pos;
    };

    Partial fillToPartial (FillResult fr);

    std::ostream& operator<<(std::ostream &os,const FillResult &fr);
    std::ostream& operator<<(std::ostream &os,const Partial &p);

}

