#pragma once

#include "zmq_rpc/client_tools.hpp"
#include "board_zmq_client/api/dma/start/post.hpp"
#include <vector>

namespace nooyan::zmq_client::api::dma::start
{
    class Handler {
    public:
        using tools_t = zmq_rpc::ClientTools;
        using post_t = Post;

        tools_t& _tools;

        explicit Handler(tools_t& tools) : _tools{ tools } {}

        post_t::Result post(post_t::Data data, int timeout = -1);
    };
}