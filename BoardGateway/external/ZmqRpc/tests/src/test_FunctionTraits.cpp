#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include "zmq_rpc/controller/function_traits.hpp"

using zmq_rpc::controller::function_traits;

template<typename T>
struct FakeHandler;

template<typename Out, typename... Args>
struct FakeHandler<Out(Args...)> {
    using output_t = Out;

    static constexpr auto lambda = [](Args...) -> output_t {
        return output_t{};
    };

    using function_t = std::function<output_t(Args...)>;

    static output_t function(Args...) {
        return output_t{};
    }

    output_t operator()(Args...) {
        return output_t{};
    }
};

template<typename... Args>
struct FakeHandler<void(Args...)> {
    using output_t = void;

    static constexpr auto lambda = [](Args...) {};

    using function_t = std::function<void(Args...)>;

    static void function(Args...) {}

    void operator()(Args...) {}
};

template<typename Out>
struct FakeHandler<Out()> {
    using output_t = Out;

    static constexpr auto lambda = []() { return output_t{}; };

    using function_t = std::function<output_t()>;

    static output_t function() { return output_t{}; }

    output_t operator()() { return output_t{}; }
};

template<>
struct FakeHandler<void()> {
    using output_t = void;

    static constexpr auto lambda = []() {};

    using function_t = std::function<output_t()>;

    static void function() {}

    void operator()() {}
};

class TestFunctionTraits : public ::testing::Test {
public:

protected:
};

TEST(TestFunctionTraits, TestHandlerWithInputAndOutput) {
    using handler_t = FakeHandler<int(int)>;
    {
        using traits = function_traits<decltype(handler_t::lambda)>;
        static_assert(std::is_same_v<traits::return_type, int>);
        static_assert(traits::arity == 1);
        static_assert(std::is_same_v<traits::arg_type<0>, int>);
    }
    {
        using traits = function_traits<handler_t::function_t>;
        static_assert(std::is_same_v<traits::return_type, int>);
        static_assert(traits::arity == 1);
        static_assert(std::is_same_v<traits::arg_type<0>, int>);
    }
    {
        using traits = function_traits<decltype(&handler_t::function)>;
        static_assert(std::is_same_v<traits::return_type, int>);
        static_assert(traits::arity == 1);
        static_assert(std::is_same_v<traits::arg_type<0>, int>);
    }
    {
        using traits = function_traits<handler_t>;
        static_assert(std::is_same_v<traits::return_type, int>);
        static_assert(traits::arity == 1);
        static_assert(std::is_same_v<traits::arg_type<0>, int>);
    }
}

TEST(TestFunctionTraits, TestHandlerWithInput) {
    using handler_t = FakeHandler<void(int)>;
    {
        using traits = function_traits<decltype(handler_t::lambda)>;
        static_assert(std::is_void_v<traits::return_type>);
        static_assert(traits::arity == 1);
        static_assert(std::is_same_v<traits::arg_type<0>, int>);
    }
    {
        using traits = function_traits<handler_t::function_t>;
        static_assert(std::is_void_v<traits::return_type>);
        static_assert(traits::arity == 1);
        static_assert(std::is_same_v<traits::arg_type<0>, int>);
    }
    {
        using traits = function_traits<decltype(&handler_t::function)>;
        static_assert(std::is_void_v<traits::return_type>);
        static_assert(traits::arity == 1);
        static_assert(std::is_same_v<traits::arg_type<0>, int>);
    }
    {
        using traits = function_traits<handler_t>;
        static_assert(std::is_void_v<traits::return_type>);
        static_assert(traits::arity == 1);
        static_assert(std::is_same_v<traits::arg_type<0>, int>);
    }
}

TEST(TestFunctionTraits, TestHandlerWithOutput) {
    using handler_t = FakeHandler<int()>;
    {
        using traits = function_traits<decltype(handler_t::lambda)>;
        static_assert(std::is_same_v<traits::return_type, int>);
        static_assert(traits::arity == 0);
    }
    {
        using traits = function_traits<handler_t::function_t>;
        static_assert(std::is_same_v<traits::return_type, int>);
        static_assert(traits::arity == 0);
    }
    {
        using traits = function_traits<decltype(&handler_t::function)>;
        static_assert(std::is_same_v<traits::return_type, int>);
        static_assert(traits::arity == 0);
    }
    {
        using traits = function_traits<handler_t>;
        static_assert(std::is_same_v<traits::return_type, int>);
        static_assert(traits::arity == 0);
    }
}

TEST(TestFunctionTraits, TestEmptyHandler) {
    using handler_t = FakeHandler<void()>;
    {
        using traits = function_traits<decltype(handler_t::lambda)>;
        static_assert(std::is_void_v<traits::return_type>);
        static_assert(traits::arity == 0);
    }
    {
        using traits = function_traits<handler_t::function_t>;
        static_assert(std::is_void_v<traits::return_type>);
        static_assert(traits::arity == 0);
    }
    {
        using traits = function_traits<decltype(&handler_t::function)>;
        static_assert(std::is_void_v<traits::return_type>);
        static_assert(traits::arity == 0);
    }
    {
        using traits = function_traits<handler_t>;
        static_assert(std::is_void_v<traits::return_type>);
        static_assert(traits::arity == 0);
    }
}

TEST(TestFunctionTraits, CheckInputType) {
    {
        using handler_t = FakeHandler<void()>;
        using first_input_t = zmq_rpc::controller::function_n_input_t<handler_t, 0>;
        static_assert(std::is_void_v<first_input_t>);
    }
    {
        using handler_t = FakeHandler<void(int)>;
        using first_input_t = zmq_rpc::controller::function_n_input_t<handler_t, 0>;
        static_assert(std::is_same_v<first_input_t, int>);
        using second_input_t = zmq_rpc::controller::function_n_input_t<handler_t, 1>;
        static_assert(std::is_void_v<second_input_t>);
    }
    {
        using handler_t = FakeHandler<void(int, float)>;
        using first_input_t = zmq_rpc::controller::function_n_input_t<handler_t, 0>;
        static_assert(std::is_same_v<first_input_t, int>);
        using second_input_t = zmq_rpc::controller::function_n_input_t<handler_t, 1>;
        static_assert(std::is_same_v<second_input_t, float>);
        using third_input_t = zmq_rpc::controller::function_n_input_t<handler_t, 2>;
        static_assert(std::is_void_v<third_input_t>);
    }
}
