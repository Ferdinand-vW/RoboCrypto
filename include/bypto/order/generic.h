#pragma once

#include "bypto/order/order.h"
#include "bypto/order/order_type.h"
#include "bypto/order/fill.h"
#include <optional>

namespace bypto::order {
        struct GenericOrder {
        private:
            template<OType T>
            auto map_generic(Order<T> o) {
                auto f = [o](auto p) -> std::optional<FillResult<GenericOrder>> {
                    auto fr_opt = fill(o,p); //
                    if(fr_opt) {
                        auto fr = fr_opt.value();
                        auto new_order_opt = fr.m_new_order;
                        if(new_order_opt) {
                            GenericOrder go(new_order_opt.value());
                            return FillResult<GenericOrder>{fr.m_partial,go};
                        } else {
                            return FillResult<GenericOrder>{fr.m_partial,std::nullopt};
                        }
                    }
                    return std::nullopt;
                };

                return f;
            }

        public:
            std::string m_type;

            Symbol m_sym;
            Quantity m_qty;
            Position m_pos;
            std::optional<TimeInForce> m_time_in_force;
            std::optional<Price> m_limit;
            std::optional<Price> m_stoptake;
            std::optional<BaseOrQuote> m_base_or_quote;

            std::function<std::optional<FillResult<GenericOrder>>(Price)> m_generic_fill;

        GenericOrder(Order<OType::Unit> o);
        GenericOrder(Order<OType::Market> m);
        GenericOrder(Order<OType::Limit> l);
        GenericOrder(Order<OType::StopLoss> sl);
        GenericOrder(Order<OType::StopLossLimit> sll);
        GenericOrder(Order<OType::TakeProfit> tp);
        GenericOrder(Order<OType::TakeProfitLimit> tpl);
        GenericOrder(Order<OType::LimitMaker> lm);
        
    };
}