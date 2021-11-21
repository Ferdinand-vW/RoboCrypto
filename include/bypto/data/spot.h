#pragma once

#include "bypto/common/types.h"
#include "bypto/common/utils.h"
#include "bypto/data/price.h"
#include "bypto/data/prices.h"

#include <bits/types/time_t.h>
#include <iostream>

namespace bypto::data::price {

    typedef Price<PriceSource::Spot> Spot_t;
    typedef prices::Prices<PriceSource::Spot> Spots_t;
    
    // Candle data
    template<> 
    class Price<PriceSource::Spot> {
        public:
            long double get_price() { return m_price; }
            long double get_quote_price() { return 1 / m_price; }
            time_t get_time() { return m_time; }
            common::types::Symbol get_symbol() { return m_symbol; }
            std::tuple<common::types::Symbol,time_t,long double> as_tuple() {
                return {get_symbol(),get_time(),get_price()};
            }

            common::types::Symbol m_symbol;
            time_t m_time; //price taken at time
            long double m_price; //price value
            
            std::string to_string() const;
    };

    std::ostream& operator<<(std::ostream&os,const price::Spot_t &kl);


}