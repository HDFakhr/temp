#pragma once

#include "framework/service_tools.hpp"
#include "framework/service_pipeline.hpp"
#include "entity/service_args.hpp"
#include <string>

namespace SERVICE_NAMESPACE_PREFIX::framework
{
    class ServiceRunner {
    public:
        static int run(const std::vector<std::string_view>& args, const entity::ServiceArgs& service_args) {
            ServiceTools tools;
            return ServicePipeline(tools)
                .load_config(args)
                .initialize(service_args.service_version)
                .initial_board_gw()
                .initial_zmq_rpc()
                .finalize();
        };
    };
}
