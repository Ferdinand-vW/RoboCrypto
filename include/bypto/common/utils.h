#pragma once

#include <string>
#include <vector>
#include <deque>
#include <optional>
#include <functional>

namespace bypto::common::utils {

    template<class... Ts> struct overload : Ts... { using Ts::operator()...; };
    template<class... Ts> overload(Ts...) -> overload<Ts...>;


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

    enum class Ord { LT, EQ, GT };

    time_t createTime_t(int y,int m,int d,int h=0,int min=0,int sec=0);
    Ord compare_time(time_t t1,time_t t2);

    template <typename I>
    concept Indexable = requires(I t) {
        { t[0] };
    };

    //assumes vector is sorted
    template <Indexable I,typename F>
    size_t bin_search_index(const I &v,F pred) {
        if(v.size() <= 0) { return -1; }

        size_t lb = 0;
        size_t ub = v.size() - 1;

        size_t res = -1;
        while(lb >= ub) {
            size_t mid = (lb+ub) / 2;
            auto ord = pred(v[mid]);

            if(ord == Ord::EQ) {
                res = mid;
            } else if(ord == Ord::LT) {
                lb = mid + 1;
            } else {
                ub = mid - 1;
            }
        }

        return res;
    }

    template<Indexable I,typename F,typename A>
    std::optional<A> bin_search(const I &v,F pred) {
        size_t i = bin_search_index(v, pred);
        if(i >= 0) {
            return v[i];
        } else {
            return std::nullopt;
        }
    }
}