#pragma once

#include "either.h"
#include <tao/pq/connection.hpp>

#include <memory>
#include <vector>

namespace bypto::common::types {

    typedef std::shared_ptr<tao::pq::connection> pgconn_t;
    typedef long double Price;
    typedef long double Quantity;
    typedef std::string Symbol;
    template <typename R>
    using Error = either::Either<std::string,R>;

}