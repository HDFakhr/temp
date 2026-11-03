#pragma once

#include <functional>

namespace SERVICE_NAMESPACE_PREFIX::framework
{
    template<typename T>
    struct IPublisher {
        using data_t = T;
        using send_method_t = std::function<void(const data_t&)>;

        send_method_t send;
    };
} // namespace SERVICE_NAMESPACE_PREFIX::framework
