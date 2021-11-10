#pragma once

#include "either.h"
#include <tao/pq/connection.hpp>

#include <memory>
#include <vector>
#include <string>

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
            bool operator<(const Symbol &rhs) const;
            static Symbol from_string(std::string s);
            std::string to_string() const;
    };

    std::ostream& operator<<(std::ostream &os,const Symbol &s);

    class Value {
        public:
            Value(std::string m_curreny,long double quantity);

            std::string m_currency;
            long double m_quantity;

            Value add(Value v1);
            Value subtract(Value v1);
            Value multiply(Value v1);
            Value divide(Value v1);

            std::string ppValue();
    };

    std::ostream& operator<<(std::ostream &os,const Value &s);
}