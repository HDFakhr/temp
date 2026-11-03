#pragma once

#include "zmq_rpc/server_tools.hpp"
#include "zmq_rpc/zmq/context.hpp"

namespace zmq_rpc
{
    class ServerPipeline {
    public:
        using str = std::string;
        using str_view = std::string_view;
        using tools_t = ServerTools;
        static constexpr int no_timeout{ -1 };

        tools_t& _tools;

        explicit ServerPipeline(tools_t& tools) : _tools{ tools } {}

        template<typename Func>
        ServerPipeline& register_api(str_view path, str_view method, Func&& handler) {
            _tools.controller.register_api(path, method, std::forward<Func>(handler));
            return *this;
        }

        ServerPipeline& initial(context_t& ctx, const str& url);
        ServerPipeline& initial_thread(int timeout = no_timeout);
        ServerPipeline& start();
        ServerPipeline& stop();
    };
}