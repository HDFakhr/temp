#pragma once

#include <zmq.hpp>
#include <memory>

namespace zmq_rpc::client
{
    struct Tools {
        using str = std::string;
        using socket_t = zmq::socket_t;

        static constexpr auto socket_type = zmq::socket_type::req;

        std::shared_ptr<socket_t> socket{ nullptr };
    };
}