#pragma once

#include "framework/service_tools.hpp"
#include <string>

namespace SERVICE_NAMESPACE_PREFIX::framework
{
    class ServicePipeline {
    public:
        using str = std::string;
        using tools_t = ServiceTools;

        tools_t& _tools;

        explicit ServicePipeline(tools_t& tools) : _tools{ tools } {}

        ServicePipeline& load_config(const std::vector<std::string_view>& args);
        ServicePipeline& initialize(const str& version);
        ServicePipeline& initial_board_gw();
        ServicePipeline& initial_zmq_rpc();
        int finalize();
    };
}
