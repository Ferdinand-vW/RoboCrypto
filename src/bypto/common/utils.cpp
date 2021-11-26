#include "bypto/common/utils.h"
#include <ctime>
#include <iomanip>
#include <iostream>
#include <string>
#include <vector>

namespace bypto::common::utils {
    
    std::vector<std::string> split(std::string s,char delim) {
        std::vector<std::string> vec;
        std::string res;
        for(const auto &c : s) {
            if(c == delim && res.size() > 0) {
                vec.push_back(res);
                res = "";
            }

            if(c != delim)
                res.push_back(c);

        }

        return vec;
    }

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
        os << "{days="    + std::to_string(tu.m_days);
        os << ",hours="   + std::to_string(tu.m_hours);
        os << ",minutes=" + std::to_string(tu.m_minutes);
        os << ",seconds=" + std::to_string(tu.m_seconds) + "}";
        return os;
    }

    time_t add_time(time_t t, time_unit tu) {
        auto timeinfo = localtime(&t);
        
        return create_time(timeinfo->tm_year + 1900
                          ,timeinfo->tm_mon + 1
                          ,timeinfo->tm_mday + tu.m_days
                          //localtime may be DST, whereas the considered time
                          //will always be in UTC
                          ,timeinfo->tm_hour + tu.m_hours - timeinfo->tm_isdst
                          ,timeinfo->tm_min + tu.m_minutes
                          ,timeinfo->tm_sec + tu.m_seconds);
    }

    time_unit diff_time(time_t t2,time_t t1) {
        int dsecs = difftime(t2, t1);

        int min_in_seconds = 60;
        int hour_in_seconds = min_in_seconds*60;
        int day_in_seconds = 24*hour_in_seconds;

        int ddays = std::floor(dsecs / day_in_seconds);
        dsecs = dsecs - ddays*day_in_seconds;
        int dhours = std::floor(dsecs / hour_in_seconds);
        dsecs = dsecs - dhours*hour_in_seconds;
        int dmins = std::floor(dsecs / min_in_seconds);
        dsecs = dsecs - dmins*min_in_seconds;
        return time_unit{ddays,dhours,dmins,dsecs};
    }

    std::string to_string(const time_unit &tu) {
        std::string res("");
        if(tu.m_days > 0)    { res += std::to_string(tu.m_days)   +"d"; }
        if(tu.m_hours > 0)   { res += std::to_string(tu.m_hours)  +"h"; }
        if(tu.m_minutes > 0) { res += std::to_string(tu.m_minutes)+"m"; }
        if(tu.m_seconds > 0) { res += std::to_string(tu.m_seconds)+"s"; }
        return res;
    }

    bool operator==(const time_unit &tu1,const time_unit &tu2) {
        return
            tu1.m_seconds == tu2.m_seconds &&
            tu1.m_minutes == tu2.m_minutes &&
            tu1.m_hours == tu2.m_hours     &&
            tu1.m_days == tu2.m_days;
    }

    bool operator!=(const time_unit &tu1, const time_unit &tu2) {
        return !(tu1 == tu2);
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

    std::string pp_time(time_t t) {
        auto timeinfo = std::localtime(&t);

        std::ostringstream oss;
        oss << std::put_time(timeinfo, "%d-%m-%Y %H-%M-%S");
        return oss.str();
    }
}