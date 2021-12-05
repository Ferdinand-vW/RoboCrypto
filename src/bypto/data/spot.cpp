#include "bypto/data/spot.h"
#include "bypto/common/csv.h"
#include "bypto/common/types.h"
#include "bypto/common/utils.h"
#include "bypto/data/price.h"
#include "tao/pq/connection.hpp"

#include <algorithm>
#include <iomanip>
#include <iterator>
#include <ostream>
#include <sstream>
#include <string>

namespace bypto::data::price {

std::string Spot_t::to_string() const {
    std::stringstream ss;
    ss << "Spot {";
    ss << " m_symbol=" << m_symbol;
    ss << ",m_time=" << std::put_time(localtime(&m_time), "%Y-%m-%d-%H-%M-%S");
    ss << ",m_price=" << m_price;
    ss << "}";

    return ss.str();
}

std::ostream &operator<<(std::ostream &os, const price::Spot_t &sp) {
    os << sp.to_string();
    return os;
}
} // namespace bypto::data::price