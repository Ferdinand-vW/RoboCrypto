#include "bypto/common/csv.h"

namespace bypto::common::csv {

    std::deque<std::string> tokenize(const std::string &s) {
        std::stringstream ss(s);

        std::deque<std::string> tokens;
        std::string token;
        while(std::getline(ss,token,',')) {
            tokens.push_back(token);
        }

        return tokens;
    }

}