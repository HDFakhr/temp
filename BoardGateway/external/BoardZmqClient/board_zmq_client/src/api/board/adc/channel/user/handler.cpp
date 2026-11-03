#include "board_zmq_client/api/board/adc/channel/user/handler.hpp"
#include "zmq_rpc/client_pipeline.hpp"
#include <exception>

using nooyan::zmq_client::api::board::adc::channel::user::Handler;

auto Handler::post(const Post::Data& data, int timeout) -> Post::Result {
    auto resp = zmq_rpc::ClientPipeline(_tools, timeout).send<Post::Result>(
        Post::path,
        Post::method,
        data
    );
    if (!resp.info.status)
        throw std::runtime_error(resp.info.error);
    return resp.result;
}