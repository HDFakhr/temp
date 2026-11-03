#pragma once

#include <zmq.hpp>
#include <string>
#include <memory>
#include <functional>
#include <span>
#include "zmq_rpc/thread/Ithread.hpp"

namespace zmq_rpc::server
{
    struct Tools {
        using str = std::string;
        using byte_t = char;
        using socket_t = zmq::socket_t;
        using thread_t = thread::IThread;
        using data_handler_t = std::function<str(std::span<const byte_t>)>;

        static constexpr auto socket_type = zmq::socket_type::rep;

        data_handler_t data_handler;
        std::shared_ptr<socket_t> socket{ nullptr };
        thread_t thread;
    };
}