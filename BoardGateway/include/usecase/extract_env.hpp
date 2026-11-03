#pragma once

#include <libenvpp/env.hpp>
#include <string>
#include <type_traits>

namespace SERVICE_NAMESPACE_PREFIX::usecase
{
    class ExtractEnv {
    public:
        using str = std::string;

        ExtractEnv(const str& prefix) : pre{ prefix } {}

        template<typename T>
        auto get_or(const str& name, T&& default_) -> std::decay_t<T> {
            using value_t = std::decay_t<T>;

            const auto _{ pre.register_variable<value_t>(name) };
            const auto parsed{ pre.parse_and_validate() };
            return parsed.get_or(_, std::forward<T>(default_));
        }

    private:
        env::prefix pre;
    };
}