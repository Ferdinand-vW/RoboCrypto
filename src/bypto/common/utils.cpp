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

    std::ostream& operator<<(std::ostream &os,const time_unit& tu) {
        os << "{years="   + std::to_string(tu.m_years);
        os << ",months="  + std::to_string(tu.m_months);
        os << ",days="    + std::to_string(tu.m_days);
        os << ",hours="   + std::to_string(tu.m_hours);
        os << ",minutes=" + std::to_string(tu.m_minutes);
        os << ",seconds=" + std::to_string(tu.m_seconds) + "}";
        return os;
    }

    time_t add_time(time_t t, time_unit tu) {
        auto timeinfo = localtime(&t);
        
        return create_time(timeinfo->tm_year + tu.m_years + 1900
                          ,timeinfo->tm_mon + tu.m_months + 1
                          ,timeinfo->tm_mday + tu.m_days
                          //localtime may be DST, whereas the considered time
                          //will always be in UTC
                          ,timeinfo->tm_hour + tu.m_hours - timeinfo->tm_isdst
                          ,timeinfo->tm_min + tu.m_minutes
                          ,timeinfo->tm_sec + tu.m_seconds);
    }

    time_t create_time(int y,int m,int d,int h,int min,int sec) {
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

    Ord compare_time(time_t t1, time_t t2) {
        if(t1 == t2) { return Ord::EQ; }
        else if(t1 < t2) { return Ord::LT;}
        else {return Ord::GT;}
    }
}