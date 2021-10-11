#pragma once

#include "bypto/common/types.h"

#include <ostream>
#include <variant>

namespace bypto::order_type {
    using namespace bypto::common::types;

/************************Base types for constructing order types****************/

    enum class Position    { Buy ,Sell };
    enum class TimeInForce { GTC ,IOC, FOK };
    enum class BaseOrQuote { Base,Quote    };

    std::ostream& operator<<(std::ostream &os,const Position &p);
    std::ostream& operator<<(std::ostream &os,const TimeInForce &p);
    std::ostream& operator<<(std::ostream &os,const BaseOrQuote &p);

/****************************Order fill types******************************/

    struct FillResult;

    class Fillable {
            virtual std::optional<FillResult> try_fill(Position pos,Price price) = 0;
    };


/*********************Concrete Order Types**********************/

    class Market : Fillable {
        public:
            Market(Quantity qty,BaseOrQuote boq);

            Quantity m_qty;
            BaseOrQuote m_boq;

            std::optional<FillResult> try_fill(Position pos,Price price);
    };

    std::ostream& operator<<(std::ostream &os,const Market &m);

    class Limit : Fillable {
        public:
            Limit(TimeInForce tif,Quantity qty,Price price);
            
            TimeInForce m_time_in_force;
            Quantity m_qty;
            Price m_price;

            std::optional<FillResult> try_fill(Position pos,Price price);
    };

    std::ostream& operator<<(std::ostream &os,const Limit &l);

    struct StopLoss : Fillable {
        Quantity m_qty;
        Price m_stop_price;

        std::optional<FillResult> try_fill(Position pos,Price price);
    };

    std::ostream& operator<<(std::ostream &os,const StopLoss &sl);

    struct StopLossLimit : Fillable {
        TimeInForce m_time_in_force;
        Quantity m_qty;
        Price m_price;
        Price m_stop_price;

        std::optional<FillResult> try_fill(Position pos,Price price);
    };

    std::ostream& operator<<(std::ostream &os,const StopLossLimit &sll);

    struct TakeProfit : Fillable {
        Quantity m_qty;
        Price m_stop_price;

        std::optional<FillResult> try_fill(Position pos,Price price);
    };

    std::ostream& operator<<(std::ostream &os,const TakeProfit &tp);

    struct TakeProfitLimit : Fillable {
        TimeInForce m_time_in_force;
        Quantity m_qty;
        Price m_price;
        Price m_stop_price;

        std::optional<FillResult> try_fill(Position pos,Price price);
    };

    std::ostream& operator<<(std::ostream &os,const TakeProfitLimit &tpl);

    struct LimitMaker : Fillable {
        Quantity m_qty;
        Price m_price;

        std::optional<FillResult> try_fill(Position pos,Price price);
    };

    std::ostream& operator<<(std::ostream &os,const LimitMaker &lm);

    typedef std::variant<Market,Limit,StopLoss,StopLossLimit,TakeProfit,TakeProfitLimit,LimitMaker> OrderType;

    std::ostream& operator<<(std::ostream &os,const OrderType &ot);

/****************************Order fill types******************************/

    struct FillResult {
        Price m_price;
        Quantity m_qty;
        Position m_pos;
        std::optional<OrderType> m_new_order;
    };

    struct Partial {
        Price m_price;
        Quantity m_qty;
        Position m_pos;
    };

    Partial fillToPartial (FillResult fr);

    std::ostream& operator<<(std::ostream &os,const FillResult &fr);
    std::ostream& operator<<(std::ostream &os,const Partial &p);

}

