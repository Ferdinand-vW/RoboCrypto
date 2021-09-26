#pragma once

#include <type_traits>
#include <variant>
#include <memory>
#include <iostream>
#include <functional>

namespace bypto::common::either {

    template<typename L,typename R>
    class Either {
        bool m_is_left = false;
        std::unique_ptr<L> m_l;
        std::unique_ptr<R> m_r;
        
        private:
            void release() {
                m_l.release();
                m_r.release();
            }

        public:
            Either(L &&l,typename std::enable_if<!std::is_same<L,R>::value,int>::type = 0) {
                m_l = std::make_unique<L>(std::move(l));
                m_is_left = true;
            }

            Either(L &l,typename std::enable_if<!std::is_same<L,R>::value,int>::type = 0) {
                m_l = std::make_unique<L>(l);
                m_is_left = true;
            };

            Either(R &&r,typename std::enable_if<!std::is_same<L,R>::value,int>::type = 0) {
                m_r = std::make_unique<R>(std::move(r));
            }

            Either(R &r,typename std::enable_if<!std::is_same<L,R>::value,int>::type = 0) {
                m_r = std::make_unique<R>(r);
            }

            Either(Either<L,R> &&e) {
                if(e.isLeft()) {
                    m_l = std::make_unique<L>(std::move(e.left()));
                    e.release();
                    m_is_left = true;
                } else {
                    m_r = std::make_unique<R>(std::move(e.right()));
                    e.release();
                }
            }

            Either(Either<L,R> &e) {
                if(e.isLeft()) {
                    m_l = std::make_unique<L>(e.left());
                    m_is_left = true;
                } else {
                    m_r = std::make_unique<R>(e.right());
                }
            }

            bool operator==(Either<L,R> &e) {
                if(m_is_left && e.m_is_left) {
                    return *m_l == e.left();
                } else if(!m_is_left && !e.m_is_left) {
                    return *m_r == e.right();
                } else {
                    return false;
                }
            }

            bool operator<(Either<L,R> &e) {
                if(m_is_left && e.m_is_left) {
                    return *m_l < e.left();
                } else if(!m_is_left && !e.m_is_left) {
                    return *m_r < e.right();
                } else {
                    return false;
                }
            }

            bool isLeft() {
                return m_is_left;
            }

            bool isRight() {
                return !m_is_left;
            }

            L& left() {
                return *m_l;
            }
            R& right() {
                return *m_r;
            }

            void mapL(std::function<L(L&)> f) {
                if(m_is_left) {
                    m_l = std::make_unique<L>(f(*m_l));
                }
            }

            void mapR(std::function<R(R&)> f) {
                if(!m_is_left) {
                    m_r = std::make_unique<R>(f(*m_r));
                }
            }

            template <typename A>
            Either<A,R> fmap(std::function<A(L&)> f) {
                if(m_is_left) {
                    return Either<A,R>(f(*m_l));
                } else {
                    return Either<A,R>(*m_r);
                }
            }

            template <typename B>
            Either<L,B> fmap(std::function<B(R&)> f) {
                if(!m_is_left) {
                    return Either<L,B>(f(*m_r));
                } else {
                    return Either<L,B>(*m_l);
                }
            }
    };

}