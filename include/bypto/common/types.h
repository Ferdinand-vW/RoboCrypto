#pragma once

#include "either.h"
#include <tao/pq/connection.hpp>

#include <memory>
#include <vector>

namespace bypto::common::types {

    typedef std::shared_ptr<tao::pq::connection> pgconn_t;
    typedef long double Price;
    typedef long double Quantity;
    template <typename R>
    using Error = either::Either<std::string,R>;

    class Symbol {
        std::string m_base;
        std::string m_quote;

        public:
            Symbol(std::string base,std::string quote);

            std::string base() const;
            std::string quote() const;

            bool operator==(const Symbol &s1) const;
            static Symbol from_string(std::string s);
            std::string to_string();
    };

    std::ostream& operator<<(std::ostream &os,const Symbol &s);
}