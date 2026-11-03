#include "board_zmq_client/api/dma/data/handler.hpp"
#include "zmq_rpc/controller/response.hpp"
#include "zmq_rpc/client_pipeline.hpp"
#include <exception>
#include <span>

using nooyan::zmq_client::api::dma::data::Handler;

auto Handler::data(get_t::Data data, int timeout) -> get_t::Result {
    get_t::Result resp;

    auto msg = zmq_rpc::ClientPipeline(_tools, timeout).send_and_get_msg(
        get_t::path,
        get_t::method,
        data
    );

    auto data_len = msg.size() / sizeof(short);
    auto data_view = std::span(static_cast<short*>(msg.data()), data_len);
    resp.assign(std::move_iterator(data_view.begin()), std::move_iterator(data_view.end()));
    return resp;
}