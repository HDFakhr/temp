#pragma once

#include <functional>

namespace SERVICE_NAMESPACE_PREFIX::framework
{
    struct ISubscriber {
        using start_method_t = std::function<void()>;
        using stop_method_t = std::function<void()>;

        start_method_t start{ nullptr };
        stop_method_t stop{ nullptr };
    };
}