#pragma once

#include <functional>

// Source: https://stackoverflow.com/a/26768388/9855653

namespace parsecpp::functional {

    template <typename FUNCTION>
    struct _curry;

    // specialization for functions with a single argument
    template <typename R,typename T>
    struct _curry<std::function<R(T)>> 
    {
        using type = std::function<R(T)>;

        const type result;

        _curry(type fun) 
            : result(fun) { }

    };

    // recursive specialization for functions with more arguments
    template <typename R,typename T,typename...Ts>
    struct _curry<std::function<R(T,Ts...)>> 
    {
        using remaining_type = typename _curry<std::function<R(Ts...)> >::type;

        using type = std::function<remaining_type(T)>;

        const type result;

        _curry(std::function<R(T,Ts...)> fun)
        : result (
            [=](const T& t) {
                return _curry<std::function<R(Ts...)>>(
                    [=](const Ts&...ts){ 
                        return fun(t, ts...); 
                    }
                ).result;
            }
        ) { }
    };

    template <typename R,typename...Ts>
    auto curry(const std::function<R(Ts...)> fun)
        -> typename _curry<std::function<R(Ts...)>>::type
    {
        return _curry<std::function<R(Ts...)>>(fun).result;
    }

    template <typename R,typename...Ts>
    auto curry(R(* const fun)(Ts...))
        -> typename _curry<std::function<R(Ts...)>>::type
    {
        return _curry<std::function<R(Ts...)>>(fun).result;
    }
}
