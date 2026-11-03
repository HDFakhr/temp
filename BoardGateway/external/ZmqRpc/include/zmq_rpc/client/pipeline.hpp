#pragma once

#include <string>
#include "zmq_rpc/client/tools.hpp"
#include "zmq_rpc/zmq/context.hpp"

namespace zmq_rpc::client
{
    class Pipeline {
    public:
        using str = std::string;
        using str_view = std::string_view;
        using tools_t = Tools;
        using message_t = zmq::message_t;
        static constexpr int no_timeout{ -1 };

        tools_t& _tools;

        explicit Pipeline(tools_t& tools) : _tools{ tools } {}

        Pipeline& initial(context_t& ctx, const str& url);
        message_t send(str_view msg, int timeout = no_timeout); // timeout in ms
    };
}