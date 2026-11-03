#pragma once

#include "zmq_rpc/client_tools.hpp"
#include "board_zmq_client/api/board/adc/channel/user/post.hpp"

namespace nooyan::zmq_client::api::board::adc::channel::user
{
    class Handler {
    public:
        using tools_t = zmq_rpc::ClientTools;
        using post_t = Post;

        tools_t& _tools;

        explicit Handler(tools_t& tools) : _tools{ tools } {}

        post_t::Result post(const post_t::Data& data, int timeout = -1);
    };
}