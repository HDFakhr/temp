#pragma once

#include <string>
#include "zmq_rpc/server/tools.hpp"
#include "zmq_rpc/zmq/context.hpp"

namespace zmq_rpc::server
{
    class Pipeline {
    public:
        using str = std::string;
        using tools_t = Tools;
        static constexpr int no_timeout{ -1 };

        tools_t& _tools;

        explicit Pipeline(tools_t& tools) : _tools{ tools } {}

        Pipeline& initial(context_t& ctx, const str& url);
        Pipeline& initial_thread(int timeout = -1); // timeout in ms
        Pipeline& start();
        Pipeline& stop();
    };
}