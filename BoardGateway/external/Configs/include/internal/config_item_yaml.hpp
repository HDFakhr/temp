#pragma once

#include <string>
#include <vector>
#include <fstream>
#include <iostream>
#include <type_traits>

#include <yaml-cpp/yaml.h>

#include "internal/utils.hpp"

namespace configs::internal::yaml
{
    template <typename T>
    class ConfigItemYaml
    {
        std::string& _key;
        T& _value;

    protected:
        ConfigItemYaml(std::string& key, T& value) : _key(key), _value(value) {}

        void load(const YAML::Node& yaml_node, bool silent)
        {
            // is supported type? (E is supported type in std::vector<E> ?)
            if constexpr (is_supported_type<std::decay_t<T>>::value)
            {
                try {
                    if (auto found_node = find_key_in_node(yaml_node, _key, silent); found_node)
                        _value = found_node.as<T>();
                }
                catch (const std::exception&) { warn_value_notvalid(_key, silent); }
            }
            // is vector? (vector of supported type was in previous 'if')
            else if constexpr (is_std_vector<std::decay_t<T>>::value)
            {
                using vector_element_type = typename std::decay_t<T>::value_type;

                if constexpr (is_supported_type<std::decay_t<vector_element_type >>::value)
                {
                    try {
                        if (auto found_node = find_key_in_node(yaml_node, _key, silent); found_node)
                            _value = found_node.as<T>();
                    }
                    catch (const std::exception&) { warn_value_notvalid(_key, silent); }
                }
                else if constexpr (std::is_same<vector_element_type, std::filesystem::path>::value)
                {
                    try {
                        if (auto found_node = find_key_in_node(yaml_node, _key, silent); found_node)
                            for (auto& path : found_node.as<std::vector<std::string>>())
                                _value.push_back(std::filesystem::path(path));
                    }
                    catch (const std::exception&) { warn_value_notvalid(_key, silent); }
                }
                else if constexpr (has_get_members_method_v<vector_element_type>)
                {
                    try {
                        if (auto found_node = find_key_in_node(yaml_node, _key, silent); found_node)
                        {
                            const auto& yaml_array = found_node;
                            _value.resize(yaml_array.size());

                            for (std::size_t i = 0; i < yaml_array.size(); ++i)
                            {
                                for_each(vector_element_type::get_members(), [&](auto&& member)
                                    { (_value[i].*member).load(yaml_array[i], silent); });
                            }
                        }
                    }
                    catch (const std::exception&) { warn_value_notvalid(_key, silent); }
                }
                else
                    static_assert(false, "The type that you given as T in ConfigItem<std::vector<T>>"
                        " is a non-primitive type and must have a get_members() method.");
            }
            // is std::filesystem::path ?
            else if constexpr (std::is_same<T, std::filesystem::path>::value)
            {
                if (auto found_node = find_key_in_node(yaml_node, _key, silent); found_node)
                    _value = std::filesystem::path(found_node.as<std::string>());
            }
            // is a object
            else if constexpr (has_get_members_method_v<T>)
            {
                if (auto found_node = find_key_in_node(yaml_node, _key, silent); found_node)
                    for_each(T::get_members(), [&](auto&& member)
                        { (_value.*member).load(found_node, silent); });
            }
            else
                static_assert(false, "The type that you given as T in ConfigItem<T> is a struct and"
                    " must have a get_members() method.");
        }

    private:
        inline void warn_key_notfound(std::string key, bool silent = false)
        {
            if (!silent)
                std::cerr << "field \'" << key << "\' not found in YAML file." << std::endl;
        }

        inline void warn_value_notvalid(std::string key, bool silent = false)
        {
            if (!silent)
                std::cerr << "value of field \'" << key << "\' isn't valid in YAML file." << std::endl;
        }

        inline auto find_key_in_node(const YAML::Node& yaml_node, std::string& key,
            bool silent_warn = false)
        {
            auto ret = yaml_node[key];
            if (!ret)
                warn_key_notfound(key, silent_warn);
            return ret;
        }
    };
}
