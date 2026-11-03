#pragma once

#include "entity/service_config.hpp"

#include <configs.hpp>
#include <zmq.hpp>
#include <zmq_rpc/server_pipeline.hpp>
#include <board_gw/frz/gateway.hpp>

#include <atomic>
#include <memory>

namespace SERVICE_NAMESPACE_PREFIX::framework
{
    struct ServiceTools {
        using config_t = entity::ServiceConfig;
        using zmq_rpc_server_t = zmq_rpc::ServerPipeline;
        using board_gateway_t = pooyan::board::frz::Gateway;

        std::atomic_bool reset_flag{ false };

        configs::ConfigItem <config_t> config{ "" };
        std::shared_ptr<zmq::context_t> zmq_context{ nullptr };
        zmq_rpc::ServerTools zmq_rpc_tools;
        std::shared_ptr<board_gateway_t> board_gateway{ nullptr };
    };
}
