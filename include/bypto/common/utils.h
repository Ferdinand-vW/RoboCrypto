#pragma once

#include <string>
#include <vector>
#include <deque>

namespace bypto::common::utils {


    template <typename Iter>
    std::string intercalate(std::string del,Iter &&begin,Iter &&end) {
        std::string str;

        if(begin != end) {
            auto s = *begin;
            str += s;
            begin++;
        }

        for(;begin != end;begin++) {
            str += del + *begin;
        }
        return str;
    }

    std::string intercalate(std::vector<std::string> &v);
    std::string intercalate(std::deque<std::string> &v);
    std::string intercalate(std::string del,std::vector<std::string> &v);
    std::string intercalate(std::string del,std::deque<std::string> &v);


    // std::string intercalate(std::string s,std::deque<std::string> &v);

    time_t createTime_t(int y,int m,int d,int h=0,int min=0,int sec=0);
}