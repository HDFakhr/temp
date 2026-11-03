#pragma once

#include <functional>

namespace zmq_rpc::thread
{
    struct IThread {
        using start_method_t = std::function<void()>;
        using stop_method_t = std::function<void()>;
        using valid_method_t = std::function<bool()>;

        start_method_t start{};
        stop_method_t stop{};
        valid_method_t valid{};
    };
}