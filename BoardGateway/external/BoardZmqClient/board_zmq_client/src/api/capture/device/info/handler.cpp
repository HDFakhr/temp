#include "board_zmq_client/api/capture/device/info/handler.hpp"
#include "zmq_rpc/controller/response.hpp"
#include "zmq_rpc/client_pipeline.hpp"
#include <exception>
#include <span>

using nooyan::zmq_client::api::capture::device::info::Handler;

auto Handler::get(int timeout ) -> get_t::Result {
    auto resp = zmq_rpc::ClientPipeline(_tools, timeout).send<get_t::Result>(
        get_t::path,
        get_t::method
    );
    if (!resp.info.status)
        throw std::runtime_error(resp.info.error);
    return resp.result;

}
