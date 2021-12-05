#pragma once

#include <deque>
#include <iostream>
#include <istream>
#include <memory>
#include <sstream>
#include <tuple>
#include <vector>

#include "bypto/common/std.h"
#include "bypto/common/utils.h"

namespace bypto::common::csv {

template <typename T> class CsvConvert {
  public:
    virtual ~CsvConvert(){};
    virtual std::stringstream to_csv() const = 0;
    virtual std::unique_ptr<T> from_csv(std::stringstream &ss) const = 0;
};

// Separates the input string by comma
std::deque<std::string> tokenize(const std::string &s);

template <typename T> T parseWord(std::string s) {
    std::stringstream word(s);
    T t;
    word >> t; // convert to type T

    return t;
}

template <typename T> std::tuple<T> parseLine(std::deque<std::string> &ss) {
    auto s = ss.front();
    ss.pop_front();
    return std::make_tuple<T>(parseWord<T>(s));
}

template <typename T, typename K, typename... Ts> std::tuple<T, K, Ts...> parseLine(std::deque<std::string> &ss) {
    auto s = ss.front();
    ss.pop_front();
    auto t = parseWord<T>(s);

    auto tpl = parseLine<K, Ts...>(ss);

    return std::tuple_cat(std::make_tuple(t), tpl);
}

template <typename... Ts> std::vector<std::tuple<Ts...>> parse(std::istream &is) {
    std::vector<std::tuple<Ts...>> records;

    std::string str;
    while (std::getline(is, str)) {
        auto toks = tokenize(str);
        auto line = parseLine<Ts...>(toks);
        records.push_back(line);
    }

    return records;
}

template <typename... Ts>
std::ostream &writeHeader(const std::array<std::string, sizeof...(Ts)> &header, std::ostream &os) {
    bool first = true;
    for (auto h : header) {
        if (first) {
            os << h;
            first = false;
        } else {
            os << "," << h;
        }
    }

    return os;
}

template <typename... Ts> std::ostream &writeRow(const std::tuple<Ts...> &data, std::ostream &os) {
    bool first = true;

    auto f = [&os, &first](auto t) {
        if (first) {
            os << t;
            first = false;
        } else {
            os << "," << t;
        }
    };
    std::apply([f](const auto &...t) { (f(t), ...); }, data);
    return os;
}

template <typename... Ts>
std::ostream &write(const std::array<std::string, sizeof...(Ts)> &header, const std::vector<std::tuple<Ts...>> &data,
                    std::ostream &os) {
    writeHeader<Ts...>(header, os) << "\n";
    for (auto &t : data) {
        writeRow(t, os) << "\n";
    }

    return os;
}
} // namespace bypto::common::csv