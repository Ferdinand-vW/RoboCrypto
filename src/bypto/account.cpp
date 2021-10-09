#include "bypto/account.h"

namespace bypto::account {
    
    void Account::add_fund(std::string s,long double qty) {
        if(m_funds.contains(s)) {
            m_funds[s]+=qty;
        } else {
            m_funds.insert({s,qty});
        }
    }
    
}