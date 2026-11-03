#pragma once

#include <nlohmann/json.hpp>
#include <optional>

namespace zmq_rpc::json
{
    using json = ::nlohmann::json;

    template<typename T>
    struct is_string : std::false_type {};

    template<>
    struct is_string<std::string> : std::true_type {};

    template<typename T>
    static constexpr auto is_string_v = is_string<T>::value;

    template<typename T>
    struct is_boolean : std::false_type {};

    template<>
    struct is_boolean<bool> : std::true_type {};

    template<typename T>
    static constexpr auto is_boolean_v = is_boolean<T>::value;

    template<typename T>
    struct is_vector : std::false_type {};

    template<typename T, typename Alloc>
    struct is_vector<std::vector<T, Alloc>> : std::true_type {};

    template<typename T>
    static constexpr auto is_vector_v = is_vector<T>::value;    

    template<typename T>
    [[maybe_unused]]
    void add_to_json(json& j, const char* name, const T& value, const T& default_value) {
        if (value != default_value)
            j[name] = value;
    }

    template<typename T>
    [[maybe_unused]]
    void add_to_json(json& j, const char* name, const T& value) {
        j[name] = value;
    }

    template<typename T>
    [[maybe_unused]]
    void add_to_json(json& j, const char* name, const std::optional<T>& value) {
        if (value.has_value())
            j[name] = value.value();
    }

    template<typename T>
    [[maybe_unused]]
    bool get_from_json_to(const json& j, const char* name, T& value) {
        using type = std::remove_reference_t<T>;
        static_assert(!std::is_const_v<type>);
        if constexpr (is_string_v<type>) {
            if (auto it = j.find(name); it != j.end() && !it->is_null() && it->is_string()) {
                it->get_to(value);
                return true;
            }
        }
        else if constexpr (is_boolean_v<type>) {
            if (auto it = j.find(name); it != j.end() && !it->is_null() && it->is_boolean()) {
                it->get_to(value);
                return true;
            }
        }
        else if constexpr (is_vector_v<type>) {
            if (auto it = j.find(name); it != j.end() && !it->is_null() && it->is_array()) {
                it->get_to(value);
                return true;
            }
        }
        else if constexpr (std::is_floating_point_v<type>) {
            if (auto it = j.find(name); it != j.end() && !it->is_null() && it->is_number_float()) {
                it->get_to(value);
                return true;
            }
        }
        else if constexpr (std::is_integral_v<type>) {
            if constexpr (std::is_unsigned_v<type>) {
                if (auto it = j.find(name); it != j.end() && !it->is_null() && it->is_number_unsigned()) {
                    it->get_to(value);
                    return true;
                }
            } else {
                if (auto it = j.find(name); it != j.end() && !it->is_null() && it->is_number_integer()) {
                    it->get_to(value);
                    return true;
                }
            }
        }
        else {
            if (auto it = j.find(name); it != j.end() && !it->is_null() && it->is_object()) {
                it->get_to(value);
                return true;
            }
        }
        return false;
    }

    template<typename T>
    [[maybe_unused]]
    std::optional<std::remove_reference_t<T>> get_from_json(const json& j, const char* name) {
        using type = std::remove_reference_t<T>;
        type value;
        if (get_from_json_to(j, name, value))
            return std::make_optional(std::move(value));
        return std::nullopt;
    }
}