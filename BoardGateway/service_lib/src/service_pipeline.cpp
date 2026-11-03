#include "framework/service_pipeline.hpp"
#include <sstream>
#include <filesystem>
#include <ecal/ecal.h>

using SERVICE_NAMESPACE_PREFIX::framework::ServicePipeline;
using str = typename ServicePipeline::str;

static str get_service_name(const str& service_name, const str& service_version) {
    std::ostringstream _;
    _ << service_name << " (" << service_version << ")";
    return _.str();
}

ServicePipeline& ServicePipeline::load_config(const std::vector<std::string_view>& args)
{
    _tools.config.load_from_args(args);
    return *this;
}

ServicePipeline& ServicePipeline::initialize(const str& version) 
{
    eCAL::Initialize(get_service_name(_tools.config().service_name(), version));
    return *this;
}

ServicePipeline& ServicePipeline::initial_board_gw()
{
    _tools.board_gateway = std::make_shared<tools_t::board_gateway_t>();
    return*this;
}

ServicePipeline& ServicePipeline::initial_zmq_rpc()
{
    tools_t::zmq_rpc_server_t(_tools.zmq_rpc_tools)
        .initial(*_tools.zmq_context, _tools.config().rpc_endpoint());

    tools_t::zmq_rpc_server_t(_tools.zmq_rpc_tools)
        .initial_thread(100)
        .register_api("/api/init", "POST",
            [&]() -> std::string {
                try { return "success"; }
                catch (...) { return "failed"; }
            }
        )
        ;

    return *this;
}

int ServicePipeline::finalize() 
{
    eCAL::Process::SetState(
        eCAL::Process::eSeverity::healthy,
        eCAL::Process::eSeverityLevel::level1,
        "Run"
    );

    while (eCAL::Ok()) {
        if (_tools.reset_flag.load()) {
            eCAL::Finalize();
            return -1;
        }
        eCAL::Process::SleepMS(1000);
    }

    return eCAL::Finalize();
}
