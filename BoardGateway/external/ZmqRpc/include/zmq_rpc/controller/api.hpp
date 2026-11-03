#pragma once

#include <string>
#include "zmq_rpc/json/json.hpp"

namespace zmq_rpc::controller
{
    struct Api {
        using str = std::string;

        struct Name {
            static constexpr auto path{ "path" };
            static constexpr auto method{ "method" };    
        };

        str path{};
        str method{};
    };

    static Api make_api(std::string_view path, std::string_view method) {
        Api _;
        _.path.assign(path);
        _.method.assign(method);
        return _;
    }

    static bool operator==(const Api& lhs, const Api& rhs) {
        return lhs.path == rhs.path &&
            lhs.method == rhs.method;
    }

    static void to_json(json::json& j, const Api& api) {
        json::add_to_json(j, Api::Name::path, api.path);
        json::add_to_json(j, Api::Name::method, api.method);
    }

    static void from_json(const json::json& j, Api& api) {
        json::get_from_json_to(j, Api::Name::path, api.path);
        json::get_from_json_to(j, Api::Name::method, api.method);
    }
}

namespace std
{
    template<>
    struct hash<zmq_rpc::controller::Api> {
        std::size_t operator()(const zmq_rpc::controller::Api& a) const noexcept {
            auto h1 = std::hash<std::string>{}(a.path);
            auto h2 = std::hash<std::string>{}(a.method);
            return h1 ^ (h2 << 1);
        }
    };
}