#pragma once

#include <string>
#include <span>
#include <unordered_map>
#include "zmq_rpc/controller/function_traits.hpp"
#include "zmq_rpc/controller/api.hpp"
#include "zmq_rpc/controller/api_handler.hpp"
#include "zmq_rpc/controller/response.hpp"

namespace zmq_rpc::controller
{
    class Controller {
    public:
        using str = std::string;
        using str_view = std::string_view;
        using byte_t = char;

        using api_handler_t = std::function<str(std::span<const byte_t>)>;

        template<typename Func>
        void register_api(str_view path, str_view method, Func&& handler) {
            
            auto api_handler = [&, h = std::forward<Func>(handler)](std::span<const byte_t> msg) -> str {
                return ApiHandler::invoke(msg, h);
            };

            _apis.insert_or_assign(make_api(path, method), std::move(api_handler));
        }

        str route(std::span<const byte_t> msg) {
            auto api{ deserialize(msg) };
            auto iter{ _apis.find(api) };
            if (iter == _apis.end())
                return not_found_response();
            return iter->second(msg);
        }

    private:
        std::unordered_map<Api, api_handler_t> _apis;

        Api deserialize(std::span<const byte_t> msg) {
            auto j = json::json::parse(msg.begin(), msg.end());
            return j.get<Api>();
        }

        str not_found_response() {
            constexpr std::string_view error{ "api not found" };
            json::json j = make_response(false, error);
            return j.dump();
        }
    };
}