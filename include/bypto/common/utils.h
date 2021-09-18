#pragma once

#include <string>
#include <vector>
#include <deque>

namespace bypto::common::utils {

    std::string intercalate(std::vector<std::string> &v);
    std::string intercalate(std::deque<std::string> &v);
    std::string intercalate(std::string s,std::vector<std::string> &v);
    std::string intercalate(std::string s,std::deque<std::string> &v);

    time_t createTime_t(int y,int m,int d,int h=0,int min=0,int sec=0);
}