#pragma once

#include <configs.hpp>
#include <string>

namespace SERVICE_NAMESPACE_PREFIX::entity
{
    template <typename T>
    using config_t = configs::ConfigItem<T>;

    struct ServiceConfig
    {
        config_t <std::string> service_name{ "service-name", std::string("Channelizer Service"), "Name of the service" };
        config_t <std::string> rpc_endpoint{ "rpc-endpoint", std::string("tcp://*:6001"), "endpoint for ZMQ rpc" };

        DECLARE_MEMBERS(
            &ServiceConfig::service_name,
            &ServiceConfig::rpc_endpoint
        )
    };
}
