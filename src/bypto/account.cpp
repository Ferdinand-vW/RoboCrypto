#include "bypto/account.h"

namespace bypto::account {

Account::Account(std::map<std::string, long double> funds) : m_funds(funds){};

std::map<std::string, long double> Account::get_funds() const { return m_funds; }

Error<Value> Account::value(std::string ccy, std::map<Symbol, long double> price_map) const {
    long double res = 0;
    for (auto &p : m_funds) {
        // look for the currency in the price_map, it can be either base or quote
        auto it = std::find_if(price_map.begin(), price_map.end(),
                               [p](auto &kvp) { return kvp.first.base() == p.first || kvp.first.quote() == p.first; });

        if (it != price_map.end()) {

            if (p.first == ccy) {
                // p=USDT,ccy=USDT, no conversion required
                res += p.second;
            } else if (it->first.base() == ccy) {
                // p=BTC,ccy=USDT,it=USDTBTC, divide by USDTBTC
                res += p.second / it->second;
            } else if (it->first.quote() == ccy) {
                // p=BTC,ccy=USDT,it=BTCUSDT, multiply by BTCUSDT
                res += p.second * it->second;
            }
        } else {
            return std::string("Could not find price mapping for " + p.first + " to " + ccy);
        }
    }

    return Value(ccy, res);
}

void Account::add_fund(std::string s, long double qty) {
    if (m_funds.contains(s)) {
        m_funds[s] += qty;
    } else {
        m_funds.insert({s, qty});
    }
}

void Account::add_funds(const std::map<std::string, long double> &funds) {
    for (auto &f : funds) {
        add_fund(f.first, f.second);
    }
}

long double Account::get_quantity(std::string ccy) const {
    if (m_funds.contains(ccy)) {
        return m_funds.find(ccy)->second;
    } else {
        return 0;
    }
}

std::ostream &operator<<(std::ostream &os, const Account &acc) {
    os << "Account {";
    bool first = true;
    for (auto &kvp : acc.get_funds()) {
        os << kvp.first << "=" << std::to_string(kvp.second);
        if (first) {
            os << ", ";
            first = false;
        }
    }
    os << "}";
    return os;
}

} // namespace bypto::account