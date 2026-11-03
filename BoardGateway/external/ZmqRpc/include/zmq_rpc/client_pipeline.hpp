#pragma once

#include <string>
#include "zmq_rpc/client_tools.hpp"
#include "zmq_rpc/client/pipeline.hpp"
#include "zmq_rpc/controller/request.hpp"
#include "zmq_rpc/controller/response.hpp"

namespace zmq_rpc
{
    class ClientPipeline {
    public:
        using str = std::string;
        using str_view = std::string_view;
        using tools_t = ClientTools;

        template<typename T>
        using response_t = controller::Response<T>;

        tools_t& _tools;
        int timeout{ -1 };

        explicit ClientPipeline(tools_t& tools, int timeout_ = -1) : _tools(tools), timeout{ timeout_ } {}

        ClientPipeline& initial(context_t& ctx, const str& url);

        client::Pipeline::message_t send_and_get_msg(str_view path, str_view method) {
            json::json j = controller::make_request(path, method);
            auto j_str{ j.dump() };
            return client::Pipeline(_tools).send(j_str, timeout);
        }

        template<typename T>
        client::Pipeline::message_t send_and_get_msg(str_view path, str_view method, T&& data) {
            json::json j = controller::make_request(path, method, std::forward<T>(data));
            auto j_str{ j.dump() };
            return client::Pipeline(_tools).send(j_str, timeout);
        }

        template<typename R>
        response_t<R> send(str_view path, str_view method) {
            auto msg{ send_and_get_msg(path, method) };
            auto resp_j = json::json::parse(msg.to_string_view());
            return resp_j.get<response_t<R>>();
        }

        template<typename R, typename T>
        response_t<R> send(str_view path, str_view method, T&& data) {
            auto msg{ send_and_get_msg(path, method, std::forward<T>(data)) };
            auto resp_j = json::json::parse(msg.to_string_view());
            return resp_j.template get<response_t<R>>();
        }
    };
}