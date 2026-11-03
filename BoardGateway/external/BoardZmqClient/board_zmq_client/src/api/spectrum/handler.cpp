#include "board_zmq_client/api/spectrum/handler.hpp"
#include "zmq_rpc/json/json.hpp"
#include "zmq_rpc/controller/request.hpp"
#include "zmq_rpc/client_pipeline.hpp"
#include <span>

using nooyan::zmq_client::api::spectrum::Handler;

auto Handler::get(int timeout) -> Get::Result {
    Get::Result result;

    auto msg = zmq_rpc::ClientPipeline(_tools, timeout).send_and_get_msg(
        Get::path,
        Get::method
    );

    auto data_view{ std::span(static_cast<float*>(msg.data()), msg.size() / sizeof(float)) };
    auto spec_length{ data_view.size() - 2 };

    result.data.assign(std::move_iterator(data_view.begin() + 2), std::move_iterator(data_view.end()));
    result.start = data_view[0];
    result.step = data_view[1];
    return result;
}