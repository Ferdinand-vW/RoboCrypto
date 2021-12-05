#pragma once

#include <numeric>

namespace bypto::common::math {

template <typename X, typename Y> Y interpolate(X x1, Y y1, X x2, Y y2, X x3) {
    return (y2 - y1) / (x2 - x1) * (x3 - x1) + y1;
}

template <typename Iter, typename F> long double average(const Iter &it, F f) {
    if (it.size() == 0) {
        return 0;
    } // prevent divide by zero

    long double sum = 0;
    for (auto &x : it) {
        sum += f(x);
    }

    return sum / it.size();
}

template <typename Iter> long double average(const Iter it) {
    return average(it, [](auto &x) { return x; });
}
} // namespace bypto::common::math