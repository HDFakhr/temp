#pragma once

#include <string>
#include <vector>
#include <fstream>
#include <iostream>
#include <type_traits>

#include <nlohmann/json.hpp>

#include "internal/utils.hpp"

namespace configs::internal::json
{
    template <typename T>
    class ConfigItemJson
    {
        std::string& _key;
        T& _value;
        
    protected:
        ConfigItemJson(std::string& key, T& value) : _key(key), _value(value) {}

        void load(const nlohmann::json& json_node, bool silent)
        {
            // is supported type? (E is supported type in std::vector<E>?)
            if constexpr (is_supported_type<std::decay_t<T>>::value)
            {
                try {
                    if (find_key_in_node(json_node, _key, silent))
                        _value = json_node.at(_key).get<T>();
                }
                catch (const std::exception&) { warn_value_notvalid(_key, silent); }
            }
            // is vector? (vector of supported type was in previous 'if')
            else if constexpr (is_std_vector<std::decay_t<T>>::value)
            {
                using vector_element_type = typename std::decay_t<T>::value_type;

                if constexpr (is_supported_type<std::decay_t<vector_element_type>>::value)
                {
                    try {
                        if (find_key_in_node(json_node, _key, silent))
                            _value = json_node.at(_key).get<T>();
                    }
                    catch (const std::exception&) { warn_value_notvalid(_key, silent); }
                }
                else if constexpr (std::is_same<vector_element_type, std::filesystem::path>::value)
                {
                    try {
                        if (find_key_in_node(json_node, _key, silent))
                            for (auto& path : json_node.at(_key).get<std::vector<std::string>>())
                                _value.push_back(std::filesystem::path(path));
                    }
                    catch (const std::exception&) { warn_value_notvalid(_key, silent); }
                }
                else if constexpr (has_get_members_method_v<vector_element_type>)
                {
                    try {
                        if (find_key_in_node(json_node, _key, silent))
                        {
                            const auto& json_array = json_node.at(_key);
                            _value.resize(json_array.size());

                            for (std::size_t i = 0; i < json_array.size(); ++i)
                            {
                                for_each(vector_element_type::get_members(), [&](auto&& member)
                                    { (_value[i].*member).load(json_array[i], silent); });
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
                try {
                    if (find_key_in_node(json_node, _key, silent))
                        _value = std::filesystem::path(json_node.at(_key).get<std::string>());
                }
                catch (const std::exception&) { warn_value_notvalid(_key, silent); }
            }
            // is a object
            else if constexpr (has_get_members_method_v<T>)
            {
                try {
                    if (find_key_in_node(json_node, _key, silent))
                        for_each(T::get_members(), [&](auto&& member)
                            { (_value.*member).load(json_node.at(_key), silent); });
                }
                catch (const std::exception&) { warn_value_notvalid(_key, silent); }
            }
            else
                static_assert(false, "The type that you given as T in ConfigItem<T> is a struct and"
                    " must have a get_members() method.");
        }

    private:
        inline void warn_key_notfound(std::string key, bool silent = false)
        {
            if (!silent)
                std::cerr << "field \'" << key << "\' not found in JSON file." << std::endl;
        }

        inline void warn_value_notvalid(std::string key, bool silent = false)
        {
            if (!silent)
                std::cerr << "value of field \'" << key << "\' isn't valid in JSON file." << std::endl;
        }

        inline bool find_key_in_node(const nlohmann::json& json_node, std::string& key,
            bool silent_warn = false)
        {
            return json_node.contains(key) ? true : (warn_key_notfound(key, silent_warn), false);
        }
    };
}
