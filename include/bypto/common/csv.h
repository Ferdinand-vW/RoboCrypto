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

    template<typename ...Ts>
    std::ostream& writeHeader(std::array<std::string,sizeof...(Ts)> header,std::ostream &os) {
        bool first = true;
        for(auto h : header) {
            if(first) {
                os << h;
            }
            else {
                os << "," << h;
            }
        }

        return os;
    }

        template<typename T,typename ...Ts>
    std::ostream& writeRow(std::tuple<T,Ts...> &data,std::ostream &os) {
        bool first = true;
        
        os << std::get<0>(data);
        auto f = [&os,&first](auto t) {
            if(first) {
                os << t;
            } else {
                os << "," << t;
            }
        };
        std::apply([f](const auto&...t){(f(t),...);}, data);
        return os;
    }

    template<typename ...Ts>
    std::ostream & write(std::array<std::string,sizeof...(Ts)> header,std::vector<std::tuple<Ts...>> &data,std::ostream & os) {
        writeHeader<Ts...>(header,os) << "\n";
        for(auto &t : data) {
            writeRow(t,os) << "\n";
        }

        return os;
    }
}