#pragma once

#include <string>
#include <vector>
#include <fstream>
#include <sstream>
#include <iostream>
#include <type_traits>

#include <CLI/CLI.hpp>

#include "internal/utils.hpp"

namespace configs::internal::args
{
    template <typename T>
    class ConfigItemArgs
    {
        std::string& _key;
        T& _value;
        std::string& _description;
        
    protected:
        ConfigItemArgs(std::string& key, T& value, std::string& description)
        : _key(key), _value(value), _description(description) { }
        
        std::shared_ptr<CLI::App> _app;

    public:
        // Register this item as a root configuration.
        void add_to_args(CLI::App& app)
        {
            if constexpr (has_get_members_method_v<T>)
            {
                const auto prefix = normalize_key(_key);

                for_each(T::get_members(), [&](auto member)
                    {
                        (_value.*member).add_to_args(app, prefix);
                    });
            }
            else
            {
                add_option(app, make_option_name("", _key), _value, _description);
            }
        }

        // Register this item as a child of another configuration.
        void add_to_args(CLI::App& app, const std::string& parent_prefix)
        {
            const auto name = make_option_name(parent_prefix, _key);

            if constexpr (has_get_members_method_v<T>)
            {
                for_each(T::get_members(), [&](auto member)
                    {
                        (_value.*member).add_to_args(app, name);
                    });
            }
            else
            {
                add_option(app, name, _value, _description);
            }
        }

    private:
        static std::string normalize_key(std::string key)
        {
            std::replace(key.begin(), key.end(), ' ', '-');
            std::replace(key.begin(), key.end(), '_', '-');
            return key;
        }

        static std::string make_option_name(const std::string& parent, const std::string& key)
        {
            const auto name = normalize_key(key);
            return parent.empty() ? name : parent + "-" + name;
        }

        template <typename U>
        static void add_option(CLI::App& app, const std::string& name, U& value, const std::string& description)
        {
            const auto option_name = "--" + name;
            using value_type = std::decay_t<U>;

            if constexpr (has_get_members_method_v<value_type>)
            {
                static_assert(
                    !has_get_members_method_v<value_type>,
                    "Nested configuration should be registered recursively.");
            }
            else if constexpr (is_std_vector<value_type>::value)
            {
                using element_type = typename value_type::value_type;

                if constexpr (!has_get_members_method_v<element_type>)
                {
                    app.add_option(option_name, value, description);
                }
                else
                {
                    app.add_option_function<std::vector<std::string>>(
                        option_name,
                        [&value, option_name](const std::vector<std::string>& raw_values)
                        {
                            value.clear();

                            constexpr auto member_count =
                                std::tuple_size<decltype(element_type::get_members())>::value;

                            if (raw_values.size() % member_count != 0)
                                throw std::runtime_error(
                                    "Invalid number of values for " + option_name);

                            for (std::size_t i = 0; i < raw_values.size(); i += member_count)
                            {
                                element_type object;
                                std::size_t index = i;

                                for_each(element_type::get_members(), [&](auto member)
                                    {
                                        using member_type = std::decay_t<
                                            decltype((object.*member)())>;

                                        (object.*member)() = string_to_<member_type>(
                                            raw_values.at(index++));
                                    });

                                value.push_back(std::move(object));
                            }
                        },
                        description
                    )->expected(1, -1);
                }
            }
            else if constexpr (std::is_same_v<value_type, bool>)
            {
                app.add_option_function<std::vector<std::string>>(
                    option_name,
                    [&value](const std::vector<std::string>& args) {
                        value = args.empty() || args.front().empty() ? true : string_to_<bool>(args.front());
                    },
                    description
                )->expected(0, 1);
            }
            else
            {
                app.add_option(option_name, value, description);
            }
        }

        template <typename U>
        static U string_to_(const std::string& str)
        {
            if constexpr (std::is_same<U, bool>::value)
                return (str == "true" || str == "1") ? true
                : (str == "false" || str == "0") ? false
                : throw std::invalid_argument("Invalid boolean value: " + str);

            else if constexpr (std::is_same<U, int>::value)
                return std::stoi(str);

            else if constexpr (std::is_same<U, float>::value)
                return std::stof(str);

            else if constexpr (std::is_same<U, double>::value)
                return std::stod(str);

            else if constexpr (std::is_same<U, long>::value)
                return std::stol(str);

            else if constexpr (std::is_same<U, std::string>::value)
                return str;

            else if constexpr (std::is_same<U, std::filesystem::path>::value)
                return std::filesystem::path(str);

            else
                static_assert(!std::is_same<U, U>::value,
                    "Unsupported type in CLI argument conversion.");
        }
    };
}
