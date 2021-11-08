#include "bypto/common/types.h"

namespace bypto::common::types {

    Symbol::Symbol(std::string base,std::string quote) : m_base(base),m_quote(quote){};

    std::string Symbol::base() const { return m_base; }
    std::string Symbol::quote() const { return m_quote;}

    Symbol Symbol::from_string(std::string s) {
        auto base = s.substr(0,3);
        auto quote = s.substr(3,3);
        return Symbol(base,quote);
    }

    std::string Symbol::to_string() const {
        return this->base() + this->quote();
    }

    bool Symbol::operator==(const Symbol &s1) const {
        return this->base() == s1.base() && this->quote() == s1.quote();
    }

    bool Symbol::operator<(const Symbol &rhs) const {
        return this->to_string() < rhs.to_string();
    }

    std::ostream& operator<<(std::ostream &os,const Symbol &s) {
        os << s.base() << s.quote();
        return os;
    }


    Value::Value(std::string currency,long double quantity) 
                : m_currency(currency)
                , m_quantity(quantity) {};

    Value Value::add(Value v1) {
        if(m_currency != v1.m_currency) {
            return *this;
        }

        m_quantity += v1.m_quantity;

        return *this;
    }

    Value Value::subtract(Value v1) {
        if(m_currency != v1.m_currency) {
            return *this;
        }

        m_quantity -= v1.m_quantity;

        return *this;
    }

    Value Value::multiply(Value v1) {
        if(m_currency != v1.m_currency) {
            return *this;
        }

        m_quantity *= v1.m_quantity;

        return *this;
    }

    Value Value::divide(Value v1) {
        if(m_currency != v1.m_currency) {
            return *this;
        }

        m_quantity /= v1.m_quantity;

        return *this;
    }

    std::ostream& operator<<(std::ostream &os,const Value &v) {
        os << "{" << v.m_currency << "," << v.m_quantity << "}";
        return os;
    }
}