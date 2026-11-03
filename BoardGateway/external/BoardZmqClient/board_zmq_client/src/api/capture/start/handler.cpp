#include "board_zmq_client/api/capture/start/handler.hpp"
#include "zmq_rpc/controller/response.hpp"
#include "zmq_rpc/client_pipeline.hpp"
#include <exception>
#include <span>

using nooyan::zmq_client::api::capture::start::Handler;

auto Handler::post(post_t::Data data, int timeout) -> post_t::Result {
    auto resp = zmq_rpc::ClientPipeline(_tools, timeout).send<post_t::Result>(
        post_t::path,
        post_t::method,
        data
    );
    if (!resp.info.status)
        throw std::runtime_error(resp.info.error);
    return resp.result;
}
