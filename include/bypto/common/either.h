#pragma once

#include <functional>
#include <iostream>
#include <memory>
#include <ostream>
#include <type_traits>
#include <variant>

namespace bypto::common::either {

template <typename L, typename R> class Either {
    bool m_is_left = false;
    std::unique_ptr<L> m_l;
    std::unique_ptr<R> m_r;

  private:
    void reset() {
        m_l.reset();
        m_r.reset();
    }

  public:
    // constructor pass by move (0 copy, 1 move)
    Either(L &&l, typename std::enable_if<!std::is_same<L, R>::value, int>::type = 0)
        : m_l(std::make_unique<L>(std::move(l))) {
        m_is_left = true;
    };

    // constructor pass by ref (1 copy, 0 move)
    Either(L &l, typename std::enable_if<!std::is_same<L, R>::value, int>::type = 0) : m_l(std::make_unique<L>(l)) {
        m_is_left = true;
    };

    // constructor pass by move (0 copy, 1 move)
    Either(R &&r, typename std::enable_if<!std::is_same<L, R>::value, int>::type = 0)
        : m_r(std::make_unique<R>(std::move(r))){};

    // constructor pass by ref (1 copy, 0 move)
    Either(R &r, typename std::enable_if<!std::is_same<L, R>::value, int>::type = 0) : m_r(std::make_unique<R>(r)) {
        m_r = std::make_unique<R>(r);
    };

    // move constructor
    Either(Either<L, R> &&e) {
        if (e.isLeft()) {
            m_l = std::make_unique<L>(std::move(e.left()));
            // e.reset();
            m_is_left = true;
        } else {
            m_r = std::make_unique<R>(std::move(e.right()));
            // e.reset();
        }
    }

    // copy constructor
    Either(Either<L, R> &e) {
        if (e.isLeft()) {
            m_l = std::make_unique<L>(e.left());
            m_is_left = true;
        } else {
            m_r = std::make_unique<R>(e.right());
        }
    }

    bool operator==(Either<L, R> &e) {
        if (m_is_left && e.m_is_left) {
            return *m_l == e.left();
        } else if (!m_is_left && !e.m_is_left) {
            return *m_r == e.right();
        } else {
            return false;
        }
    }

    bool operator<(Either<L, R> &e) {
        if (m_is_left && e.m_is_left) {
            return *m_l < e.left();
        } else if (!m_is_left && !e.m_is_left) {
            return *m_r < e.right();
        } else {
            return false;
        }
    }

    bool isLeft() const { return m_is_left; }

    bool isRight() const { return !m_is_left; }

    L &left() const { return *m_l; }
    R &right() const { return *m_r; }

    void mapL(std::function<L(L &)> f) {
        if (m_is_left) {
            m_l = std::make_unique<L>(f(*m_l));
        }
    }

    void mapR(std::function<R(R &)> f) {
        if (!m_is_left) {
            m_r = std::make_unique<R>(f(*m_r));
        }
    }

    template <typename A> Either<A, R> fmap(std::function<A(L &)> f) {
        if (m_is_left) {
            return Either<A, R>(f(*m_l));
        } else {
            return Either<A, R>(*m_r);
        }
    }

    template <typename B> Either<L, B> fmap(std::function<B(R &)> f) {
        if (!m_is_left) {
            return Either<L, B>(f(*m_r));
        } else {
            return Either<L, B>(*m_l);
        }
    }
};

template <typename L, typename R> Either<L, R> left(L &&l) { return Either<L, R>(std::move(l)); }

template <typename L, typename R> Either<L, R> right(R &&r) { return Either<L, R>(std::move(r)); }

template <typename L, typename R> std::ostream &operator<<(std::ostream &os, const Either<L, R> &e) {
    if (e.isLeft()) {
        os << "{L " << e.left() << " }";
    } else {
        os << "{R " << e.right() << " }";
    }

    return os;
}
} // namespace bypto::common::either