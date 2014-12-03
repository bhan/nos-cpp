#pragma once
#include <cstddef>
#include <tuple>
#include <type_traits>
#include <functional>
#include <utility>

namespace TupleFunctional {
    /*
        Code for apply_fn borrowed from http://stackoverflow.com/questions/687490/how-do-i-expand-a-tuple-into-variadic-template-functions-arguments
    */
    template<size_t N>
    struct Apply {
        template<typename F, typename T, typename... A>
        static inline auto apply(F && f, T && t, A &&... a)
            -> decltype(Apply<N-1>::apply(
                ::std::forward<F>(f), ::std::forward<T>(t),
                ::std::get<N-1>(::std::forward<T>(t)), ::std::forward<A>(a)...
            ))
        {
            return Apply<N-1>::apply(::std::forward<F>(f), ::std::forward<T>(t),
                ::std::get<N-1>(::std::forward<T>(t)), ::std::forward<A>(a)...
            );
        }
    };

    template<>
    struct Apply<0> {
        template<typename F, typename T, typename... A>
        static inline auto apply(F && f, T &&, A &&... a)
            -> decltype(::std::forward<F>(f)(::std::forward<A>(a)...))
        {
            return ::std::forward<F>(f)(::std::forward<A>(a)...);
        }
    };

    template<typename F, typename T>
    inline auto apply_fn(F && f, T && t)
        -> decltype(Apply< ::std::tuple_size<
            typename ::std::decay<T>::type
        >::value>::apply(::std::forward<F>(f), ::std::forward<T>(t)))
    {
        return Apply< ::std::tuple_size<
            typename ::std::decay<T>::type
        >::value>::apply(::std::forward<F>(f), ::std::forward<T>(t));
    }


    // MY OWN CREATION!

    template < typename FN, typename TYP, typename... ARGS >
    inline auto apply_nonstatic_fn(FN&& fn, TYP&& obj, std::tuple<ARGS...> &args)
        -> typename decltype(std::mem_fn(fn))::result_type
    {
        auto mem_fn_tuple = std::tuple_cat( std::tuple<TYP>{ obj }, args );
        return apply_fn(std::mem_fn(fn), mem_fn_tuple);
    }
};
