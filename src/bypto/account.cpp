#include "bypto/account.h"

namespace bypto::account {
    
    std::map<std::string,long double> Account::get_funds() const {
        return m_funds;
    }

    void Account::add_fund(std::string s,long double qty) {
        if(m_funds.contains(s)) {
            m_funds[s]+=qty;
        } else {
            m_funds.insert({s,qty});
        }
    }

    long double Account::get_quantity(std::string ccy) const {
        if(m_funds.contains(ccy)) {
            return m_funds.find(ccy)->second;
        } else {
            return 0;
        }
    }

    std::ostream& operator<<(std::ostream&os, const Account &acc) {
        os << "Account {";
        bool first = true;
        for(auto &kvp : acc.get_funds()) {
            os << kvp.first <<"="<<std::to_string(kvp.second);
            if(first) {
                os << ", ";
                first = false;
            }
        }
        os << "}";
        return os;
    }
    
}