#pragma once

#include <string>
#include <type_traits>
#include "zmq_rpc/controller/api.hpp"
#include "zmq_rpc/json/json.hpp"

namespace zmq_rpc::controller
{
    template<typename T>
    struct Request {
        using api_t = Api;
        using data_t = T;

        struct Name {
            static constexpr auto data{ "data" };
        };

        api_t api{};
        T data{};
    };

    template<>
    struct Request<void> {
        using api_t = Api;
        using data_t = void;

        api_t api{};
    };

    static auto make_request(std::string_view path, std::string_view method) {
        return Request<void>{ make_api(path, method) };
    }

    template<typename T>
    static auto make_request(std::string_view path, std::string_view method, T&& data) {
        using data_t = std::decay_t<T>;
        return Request<data_t>{ make_api(path, method), std::forward<T>(data) };
    }

    template<typename T>
    static void to_json(json::json& j, const Request<T>& req) {
        using data_t = std::remove_reference_t<T>;
        using req_t = Request<data_t>;

        json::add_to_json(j, req_t::api_t::Name::path, req.api.path);
        json::add_to_json(j, req_t::api_t::Name::method, req.api.method);
        if constexpr (!std::is_void_v<data_t>) {
            json::add_to_json(j, req_t::Name::data, req.data);
        }
    }

    template<typename T>
    static void from_json(const json::json& j, Request<T>& req) {
        static_assert(!std::is_const_v<T>);
        using data_t = std::remove_reference_t<T>;
        using req_t = Request<data_t>;

        json::get_from_json_to(j, req_t::api_t::Name::path, req.api.path);
        json::get_from_json_to(j, req_t::api_t::Name::method, req.api.method);
        if constexpr (!std::is_void_v<data_t>) {
            json::get_from_json_to(j, req_t::Name::data, req.data);
        }
    }
}