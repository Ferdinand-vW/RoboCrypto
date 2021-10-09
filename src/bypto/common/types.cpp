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

    std::string Symbol::to_string() {
        return this->base() + this->quote();
    }

    bool Symbol::operator==(const Symbol &s1) const {
        return this->base() == s1.base() && this->quote() == s1.quote();
    }

    std::ostream& operator<<(std::ostream &os,const Symbol &s) {
        os << s.base() << s.quote();
        return os;
    }

}