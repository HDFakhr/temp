#pragma once

#include <string>
#include <span>
#include "zmq_rpc/controller/function_traits.hpp"
#include "zmq_rpc/controller/request.hpp"
#include "zmq_rpc/controller/response.hpp"

namespace zmq_rpc::controller
{
    struct ApiHandler {
        using str = std::string;
        using byte_t = char;

        template<typename Func>
        static str invoke(std::span<const byte_t> msg, Func&& handler) {
            using func_t = std::decay_t<Func>;
            using return_t = function_return_t<func_t>;
            using input_t = function_n_input_t<func_t, 0>;

            try {
                if constexpr (std::is_void_v<input_t>) {
                    if constexpr (std::is_void_v<return_t>) {
                        std::invoke(handler);
                        return _make_response();
                    } else  {
                        return _make_response(std::invoke(handler));
                    }
                } else {
                    auto data{ _deserialize_request<input_t>(msg) };
                    if constexpr (std::is_void_v<return_t>) {
                        std::invoke(handler, std::move(data));
                        return _make_response();
                    } else  {
                        return _make_response(std::invoke(handler, std::move(data)));
                    }
                }
            } catch (const std::exception& e) {
                return _make_error_response(e.what());
            }
        }
    
    private:
        template<typename T>
        static auto _deserialize_request(std::span<const byte_t> msg) {
            using data_t = std::decay_t<T>;

            data_t _;
            auto j = json::json::parse(msg.begin(), msg.end());
            json::get_from_json_to(j, Request<data_t>::Name::data, _);
            return _;
        }    

        static str _make_response() noexcept {
            json::json _ = make_response();
            return _.dump();
        }

        template<typename T>
        static str _make_response(T&& result) {
            json::json _ = make_response(std::forward<T>(result));
            return _.dump();
        }

        static str _make_error_response(const str& e) noexcept {
            json::json _ = make_response(false, e);
            return _.dump();
        }
    };
}