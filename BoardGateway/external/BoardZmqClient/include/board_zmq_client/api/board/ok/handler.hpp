#pragma once

#include "zmq_rpc/client_tools.hpp"
#include "board_zmq_client/api/board/ok/get.hpp"
#include <vector>

namespace nooyan::zmq_client::api::board::ok
{
    class Handler {
    public:
        using tools_t = zmq_rpc::ClientTools;
        using get_t = Get;

        tools_t& _tools;

        explicit Handler(tools_t& tools) : _tools{ tools } {}

        get_t::Result get(int timeout = -1);
    };
}