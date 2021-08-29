#pragma once

#include <string>
#include <vector>
#include <deque>

namespace bypto::common::utils {

    std::string intercalate(std::vector<std::string> &v) {
        std::string str;
        for(auto &s : v) {
            str += s;
        }
        return str;
    }

    std::string intercalate(std::deque<std::string> &v) {
        std::string str;
        for(auto &s : v) {
            str += s;
        }
        return str;
    }
}