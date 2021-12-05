#pragma once

#include <optional>
#include <ostream>

namespace bypto {

template <typename A> std::ostream &operator<<(std::ostream &os, const std::optional<A> &opt) {
    if (opt) {
        os << opt.value();
    }

    return os;
}

template <typename A> A from_opt(A &a, const std::optional<A> &opt_a) {
    if (opt_a) {
        return opt_a.value();
    } else {
        return a;
    }
}

template <typename M, typename F> auto bind(M &&opt, F &&f) {

    if (opt) {
        return std::forward<F>(f)(std::forward<M>(opt).value());
    } else {
        decltype(std::forward<F>(f)(std::forward<M>(opt).value())) no;
        return no;
    }
}

} // namespace bypto