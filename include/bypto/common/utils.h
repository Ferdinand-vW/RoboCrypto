#pragma once

#include <string>
#include <vector>
#include <deque>

namespace bypto::common::utils {

    std::string intercalate(std::vector<std::string> &v);
    std::string intercalate(std::deque<std::string> &v);
    std::string intercalate(std::string s,std::vector<std::string> &v);
    std::string intercalate(std::string s,std::deque<std::string> &v);
}