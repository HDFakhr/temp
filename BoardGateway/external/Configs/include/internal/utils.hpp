#pragma once

#include <string>
#include <vector>
#include <type_traits>

namespace configs::internal
{
#define DECLARE_MEMBERS(...) \
        static constexpr auto get_members() { return std::make_tuple(__VA_ARGS__); }

#if YAML_ENABLE
    namespace yaml
    {
        template <typename T>
        struct is_supported_type
        {
            static constexpr bool value =
                std::is_fundamental<T>::value ||
                std::is_arithmetic<T>::value ||
                std::is_same<T, std::string>::value;
        };
    }
#endif

#if JSON_ENABLE
    namespace json
    {
        template <typename T>
        struct is_supported_type
        {
            static constexpr bool value =
                std::is_fundamental<T>::value ||
                std::is_arithmetic<T>::value ||
                std::is_same<T, std::string>::value;
        };
    }
#endif
    
#if ENV_ENABLE
    namespace env
    {
        template <typename T>
        struct is_supported_type
        {
            static constexpr bool value =
                std::is_same<T, bool>::value ||
                std::is_same<T, short>::value || std::is_same<T, unsigned short>::value ||
                std::is_same<T, int>::value || std::is_same<T, unsigned int>::value ||
                std::is_same<T, float>::value ||
                std::is_same<T, double>::value ||
                std::is_same<T, long>::value || std::is_same<T, unsigned long>::value ||
                std::is_same<T, long long>::value || std::is_same<T, unsigned long long>::value ||
                std::is_same<T, std::string>::value ||
                std::is_same<T, std::filesystem::path>::value;
        };

        template <typename T>
        struct is_primitive_type
        {
            static constexpr bool value =
                std::is_fundamental<T>::value ||
                std::is_arithmetic<T>::value;
        };
    }
#endif

#if ARGS_ENABLE
    namespace args
    {
        template <typename T>
        struct is_supported_type
        {
            static constexpr bool value =
                std::is_arithmetic_v<T> ||
                std::is_same_v<T, std::string> ||
                std::is_same_v<T, std::filesystem::path>;
        };
    }
#endif

    template <typename T>
    struct is_std_vector : std::false_type {};

    template <typename T, typename Alloc>
    struct is_std_vector <std::vector<T, Alloc>> : std::true_type {};

    template <typename, typename = std::void_t<>>
    struct has_get_members_method : std::false_type {};

    template <typename T>
    struct has_get_members_method<T, std::void_t<decltype(T::get_members())>> : std::true_type {};

    template <typename T>
    constexpr bool has_get_members_method_v = has_get_members_method<T>::value;


    template <typename T, typename Func, std::size_t... Is>
    constexpr void for_each_impl(T&& tuple, Func&& f, std::index_sequence<Is...>) {
        (f(std::get<Is>(tuple)), ...);
    }

    template <typename T, typename Func>
    constexpr void for_each(T&& tuple, Func&& f) {
        constexpr std::size_t N = std::tuple_size<std::decay_t<T>>::value;
        for_each_impl(std::forward<T>(tuple), std::forward<Func>(f), std::make_index_sequence<N>{});
    }

#if defined _MSVC_LANG
    struct is_MSVC_enable : std::true_type {};
#else
    struct is_MSVC_enable : std::false_type {};
#endif
}
