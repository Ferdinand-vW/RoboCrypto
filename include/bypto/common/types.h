#pragma once

#include "tao/pq/connection.hpp"
#include <memory>

namespace bypto::common::types {

    typedef std::shared_ptr<tao::pq::connection> pgconn_t;

}