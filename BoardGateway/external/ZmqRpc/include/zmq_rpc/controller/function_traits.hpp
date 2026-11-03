#pragma once

#include <type_traits>
#include <functional>

namespace zmq_rpc::controller
{
    template<typename T>
    struct function_traits;

    template<typename R, typename... Args>
    struct function_traits<R(*)(Args...)> {
        using return_type = R;
        using args_tuple = std::tuple<Args...>;
        static constexpr std::size_t arity = sizeof...(Args);

        template<std::size_t N>
        using arg_type = typename std::tuple_element<N, args_tuple>::type;
    };

    template<typename R, typename... Args>
    struct function_traits<std::function<R(Args...)>>
        : function_traits<R(*)(Args...)> {};

    template<typename C, typename R, typename... Args>
    struct function_traits<R(C::*)(Args...)>
        : function_traits<R(*)(Args...)> {};

    template<typename C, typename R, typename... Args>
    struct function_traits<R(C::*)(Args...) const>
        : function_traits<R(*)(Args...)> {};

    template<typename F>
    struct function_traits : function_traits<decltype(&F::operator())> {};

    template<typename F>
    using function_return_t = typename function_traits<F>::return_type;

    template<typename F, std::size_t N, bool HasArgs = (function_traits<F>::arity > N)>
    struct function_n_input;

    template<typename F, std::size_t N>
    struct function_n_input<F, N, true> {
        using type = typename function_traits<F>::template arg_type<N>;
    };

    template<typename F, std::size_t N>
    struct function_n_input<F, N, false> {
        using type = void;
    };
    
    template<typename F, std::size_t N>
    using function_n_input_t = function_n_input<F, N>::type;
}