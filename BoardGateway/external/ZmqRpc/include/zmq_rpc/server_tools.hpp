#pragma once

#include "zmq_rpc/server/tools.hpp"
#include "zmq_rpc/controller/controller.hpp"

namespace zmq_rpc
{
    struct ServerTools {
        using controller_t = controller::Controller;
        using server_t = server::Tools;

        controller_t controller;
        server_t server;
    };
}