#pragma once

#include "zmq_rpc/client_tools.hpp"
#include "board_zmq_client/api/spectrum/get.hpp"
#include <vector>

namespace nooyan::zmq_client::api::spectrum
{
    class Handler {
    public:
        using tools_t = zmq_rpc::ClientTools;

        tools_t& _tools;

        explicit Handler(tools_t& tools) : _tools{ tools } {}

        Get::Result get(int timeout = -1);
    };
}