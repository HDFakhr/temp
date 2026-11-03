#pragma once

#include <string>
#include <vector>
#include <fstream>
#include <sstream>
#include <iostream>
#include <type_traits>

#include <libenvpp/env.hpp>

#include "internal/utils.hpp"

namespace configs::internal::env
{
    template <typename T>
    class ConfigItemEnv
    {
        std::string& _key;
        T& _value;

    protected:
        ConfigItemEnv(std::string& key, T& value) : _key(key), _value(value) {}

        void load(const std::string env_prefix_node, bool silent)
        {
            auto full_key = get_key_with_prefix(env_prefix_node);

            // is vector?
            if constexpr (is_std_vector<std::decay_t<T>>::value)
            {
                using vector_element_type = typename std::decay_t<T>::value_type;

                if (find_key(full_key, silent))
                {
                    try {
                        _value = parse_env_vector_string<vector_element_type>(
                            ::env::get<std::string>(full_key).value());
                    }
                    catch (const std::exception&) { warn_value_notvalid(full_key, silent); }
                }
            }
            // is supported type?
            else if constexpr (is_supported_type<std::decay_t<T>>::value)
            {
                if (find_key(full_key, silent))
                {
                    if (auto env_node = ::env::get<T>(full_key); env_node.has_value())
                        _value = env_node.value();
                    else
                        warn_value_notvalid(full_key, silent);
                }
            }
            // is a object
            else if constexpr (has_get_members_method_v<T>)
            {
                for_each(T::get_members(), [&](auto&& member) {
                    (_value.*member).load(full_key, silent);
                    });
            }
            else
                static_assert(false, "The type that you given as T in ConfigItem<T> is a struct and"
                    " must have a get_members() method.");
        }

    private:
        inline std::string get_key_with_prefix(const std::string prefix)
        {
            auto uppercase = [](const std::string& str) {
                std::string result{ "" };
                for (char ch : str)
                    result += (ch == ' ' || ch == '-') ? '_' : std::toupper(ch);
                return result;
                };

            return uppercase(prefix) + (prefix.empty() || _key.empty() ? "" : "_") + uppercase(_key);
        }

        template <typename Elm>
        std::vector<Elm> parse_env_vector_string(const std::string& env_value)
        {
            std::vector<Elm> result;
            std::stringstream ss(env_value);
            std::string item;

            while (std::getline(ss, item, ';'))
                result.push_back(string_to_<Elm>(item));

            return result;
        }

        template <typename T>
        T string_to_(const std::string& str)
        {
            if constexpr (std::is_same<T, bool>::value)
                return (str == "true" || str == "1") ? true
                    : (str == "false" || str == "0") ? false
                    : throw std::invalid_argument("invalid value in boolean (true/false or 1/0)");
            else if constexpr (std::is_same<T, int>::value)
                return std::stoi(str);
            else if constexpr (std::is_same<T, float>::value)
                return std::stof(str);
            else if constexpr (std::is_same<T, double>::value)
                return std::stod(str);
            else if constexpr (std::is_same<T, long>::value)
                return std::stol(str);
            else if constexpr (std::is_same<T, std::string>::value)
                return str;
            else if constexpr (std::is_same<T, std::filesystem::path>::value)
                return std::filesystem::path(str);
            else if constexpr (is_primitive_type<T>::value)
                static_assert(false, "Unsupported type for 'string_to_<T>()' method: "
                    "the type that you given as T in ConfigItem<std::vector<T>> is a primitive type "
                    "but not handled yet."
                    );
            else if constexpr (has_get_members_method_v<T>)
            {
                using object_t = T;

                object_t obj;
                std::stringstream members_str(str);

                for_each(object_t::get_members(), [&](auto&& member) {
                    using object_member_t = std::remove_reference<decltype((obj.*member)())>::type;

                    std::string member_str;
                    std::getline(members_str, member_str, ',');

                    (obj.*member) = string_to_<object_member_t>(member_str);
                    });

                return obj;
            }
            else
                static_assert(false, "Unsupported type for 'string_to_<T>()' method: "
                    "the type that you given as T in ConfigItem<std::vector<T>> is a non-primitive "
                    "type and must have a get_members() method.");
        }

        inline void warn_key_notfound(std::string key, bool silent = false)
        {
            if (!silent)
                std::cerr << "variable \'" << key << "\' not found in Env." << std::endl;
        }

        inline void warn_value_notvalid(std::string key, bool silent = false)
        {
            if (!silent)
                std::cerr << "value of \'" << key << "\' variable is invalid." << std::endl;
        }

        inline bool find_key(std::string& key, bool silent_warn = false)
        {
            return ::env::get<std::string>(key).has_value() ? true :
                (warn_key_notfound(key, silent_warn), false);
        }
    };
}
