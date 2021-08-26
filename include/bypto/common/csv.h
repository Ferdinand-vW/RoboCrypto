#pragma once

#include <deque>
#include <iostream>
#include <istream>
#include <sstream>
#include <tuple>
#include <vector>

namespace bypto::common::csv {

    // template<typename T>
    // std::string parse(std::stringstream ss) {

    // }

    // template<typename ...Ts>
    // std::tuple<Ts...> parseLine(std::istream& is) {
    //     std::string str;
    //     std::getline(is,str);
    //     std::stringstream ss(str);

    //     std::vector<std::string> tokens;
    //     std::string token;
    //     while(std::getline(ss,token,',')) {
    //         tokens.push_back(token);
    //     }
    // }

    template <typename ...Ts>
    std::tuple<Ts...> parseLine(std::deque<std::string>&);
    template <typename T>
    T parseWord(const std::string &s);

    std::deque<std::string> tokenize(const std::string &s) {
        std::stringstream ss(s);

        std::deque<std::string> tokens;
        std::string token;
        while(std::getline(ss,token,',')) {
            tokens.push_back(token);
        }

        return tokens;
    }

    template<typename ...Ts>
    std::vector<std::tuple<Ts...>> parse(std::istream & is) {
        std::vector<std::tuple<Ts...>> records;

        std::string str;
        while(std::getline(is,str)) {
            auto line = parseLine<Ts...>(tokenize(str));
            records.push_back(line);
        }

        return records;
    }

    template <typename T,typename ...Ts>
    std::tuple<T,Ts...> parseLine(std::deque<std::string>& ss) {
        auto s = ss.front();
        ss.pop_front();
        auto t = parseWord<T>(s);

        auto tpl = parseLine<Ts...>(ss);

        return std::tuple_cat(std::make_tuple(t),tpl);
    }

    template <typename T>
    std::tuple<T> parseLine(std::deque<std::string> &ss) {
        auto s = ss.front();
        ss.pop_front();
        return std::make_tuple(parseWord<T>(s));
    }
    
    template <typename T>
    T parseWord(std::string s) {
        std::stringstream word(s);
        T t;
        word >> t;

        return t;
    }
    

}