#include "bypto/common/utils.h"
#include <vector>

namespace bypto::common::utils {
    
    std::string intercalate(std::vector<std::string > &v) {
        return intercalate("",v);
    }

    std::string intercalate(std::string del,std::vector<std::string> &v) {
        std::string str;
        auto it = v.begin();
        if(v.size() > 0) {
            auto s = *it;
            str += s;
            it++;
        }

        for(;it != v.end();it++) {
            str += del + *it;
        }
        return str;
    }

    std::string intercalate(std::deque<std::string > &v) {
        return intercalate("",v);
    }

    std::string intercalate(std::string del,std::deque<std::string> &v) {
        std::string str;

        if(v.size() > 0) {
            auto s = v.front();
            v.pop_front();
            str += s;
        }

        for(auto &s : v) {
            str += del + s;
        }
        return str;
    }
}