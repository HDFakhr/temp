#pragma once

#include <string>
#include "zmq_rpc/json/json.hpp"

namespace zmq_rpc::controller
{
    struct ResponseInfo {
        using str = std::string;

        str error{};
        bool status{ false };
    };

    template<typename T>
    struct Response {
        using info_t = ResponseInfo;
        using result_t = T;

        struct Name {
            static constexpr auto error{ "error" };
            static constexpr auto result{ "result" };
            static constexpr auto status{ "status" };
        };

        info_t info{};
        result_t result{};
    };

    template<>
    struct Response<void> {
        using info_t = ResponseInfo;
        using result_t = void;

        struct Name {
            static constexpr auto error{ "error" };
            static constexpr auto status{ "status" };
        };

        info_t info{};
    };

    template<typename T>
    static auto make_response(T&& result, bool status = true, std::string_view error = "") 
        -> Response<std::decay_t<T>> 
    {
        using result_t = std::decay_t<T>;
        Response<result_t> res;
        res.info.status = status;
        res.info.error.assign(error);
        res.result = std::forward<T>(result);
        return res;
    }

    static auto make_response(bool status = true, std::string_view error = "") 
        -> Response<void>
    {
        Response<void> res;
        res.info.status = status;
        res.info.error.assign(error);
        return res;
    }

    template<typename T>
    static void to_json(json::json& j, const Response<T>& res) {
        using result_t = std::remove_reference_t<T>;
        using res_t = Response<T>;

        json::add_to_json(j, res_t::Name::error, res.info.error);
        json::add_to_json(j, res_t::Name::status, res.info.status);
        if constexpr (!std::is_void_v<result_t>) {
            json::add_to_json(j, res_t::Name::result, res.result);
        }
    }

    template<typename T>
    static void from_json(const json::json& j, Response<T>& res) {
        using result_t = std::remove_reference_t<T>;
        using res_t = Response<T>;

        json::get_from_json_to(j, res_t::Name::error, res.info.error);
        json::get_from_json_to(j, res_t::Name::status, res.info.status);
        if constexpr (!std::is_void_v<result_t>) {
            json::get_from_json_to(j, res_t::Name::result, res.result);
        }
    }
}