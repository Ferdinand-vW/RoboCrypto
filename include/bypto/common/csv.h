#pragma once

#include <deque>
#include <iostream>
#include <istream>
#include <sstream>
#include <tuple>
#include <vector>

#include "utils.h"
namespace bypto::common::csv {

    // Separates the input string by comma
    std::deque<std::string> tokenize(const std::string &s);

    template <typename T>
    T parseWord(std::string s) {
        std::stringstream word(s);
        T t;
        word >> t; // convert to type T

        return t;
    }

    template <typename T>
    std::tuple<T> parseLine(std::deque<std::string> &ss) {
        auto s = ss.front();
        ss.pop_front();
        return std::make_tuple<T>(parseWord<T>(s));
    }

    template <typename T,typename K,typename ...Ts>
    std::tuple<T,K,Ts...> parseLine(std::deque<std::string>& ss) {
        auto s = ss.front();
        ss.pop_front();
        auto t = parseWord<T>(s);

        auto tpl = parseLine<K,Ts...>(ss);

        return std::tuple_cat(std::make_tuple(t),tpl);
    }

    template<typename ...Ts>
    std::vector<std::tuple<Ts...>> parse(std::istream & is) {
        std::vector<std::tuple<Ts...>> records;

        std::string str;
        while(std::getline(is,str)) {
            auto toks = tokenize(str);
            auto line = parseLine<Ts...>(toks);
            records.push_back(line);
        }

        return records;
    }
}