#include "bypto/common/utils.h"
#include <ctime>
#include <iostream>
#include <vector>

namespace bypto::common::utils {
    
    std::string intercalate(std::vector<std::string > &v) {
        return intercalate("",v);
    }

    std::string intercalate(std::deque<std::string > &v) {
        return intercalate("",v);
    }

    std::string intercalate(std::string del,std::vector<std::string> &v) {
        return intercalate(del,v.begin(),v.end());
    }

    std::string intercalate(std::string del,std::deque<std::string> &v) {
        return intercalate(del,v.begin(),v.end());
    }

    // std::string intercalate(std::string del,std::vector<std::string> &v) {
    //     std::string str;
    //     auto it = v.begin();
    //     if(v.size() > 0) {
    //         auto s = *it;
    //         str += s;
    //         it++;
    //     }

    //     for(;it != v.end();it++) {
    //         str += del + *it;
    //     }
    //     return str;
    // }

    // std::string intercalate(std::string del,Iter i)

    // std::string intercalate(std::deque<std::string > &v) {
    //     return intercalate("",v);
    // }

    // std::string intercalate(std::string del,std::deque<std::string> &v) {
    //     std::string str;

    //     if(v.size() > 0) {
    //         auto s = v.front();
    //         v.pop_front();
    //         str += s;
    //     }

    //     for(auto &s : v) {
    //         str += del + s;
    //     }
    //     return str;
    // }

    time_t createTime_t(int y,int m,int d,int h,int min,int sec) {
        time_t t;

        time(&t); //get current time

        auto timeinfo = localtime(&t);
        timeinfo->tm_year = y - 1900;
        timeinfo->tm_mon = m - 1;
        timeinfo->tm_mday = d;
        timeinfo->tm_hour = h;
        timeinfo->tm_min = min;
        timeinfo->tm_sec = sec;

        //this bit is taken from stackoverflow and hasn't been tested yet
        //timegm is UNIX only, but supposedly _mkgmtime should work for Windows
        //If on Windows this macro will replace timegm with _mkgmtime
        #ifdef WIN32
        # define timegm _mkgmtime
        #endif
        timegm(timeinfo); //interpret given date as UTC

        return std::mktime(timeinfo);
    }
}