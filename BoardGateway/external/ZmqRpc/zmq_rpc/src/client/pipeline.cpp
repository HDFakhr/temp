#include "zmq_rpc/client/pipeline.hpp"
#include <stdexcept>

using zmq_rpc::client::Pipeline;

Pipeline& Pipeline::initial(context_t& ctx, const str& url) {
    _tools.socket = std::make_shared<tools_t::socket_t>(ctx, tools_t::socket_type);
    _tools.socket->connect(url);
    _tools.socket->set(zmq::sockopt::immediate, 1);
    _tools.socket->set(zmq::sockopt::linger, 0);
    return *this;
}

auto Pipeline::send(str_view msg, int timeout) -> message_t {
    using message_t = zmq::message_t;

    if (!_tools.socket)
        throw std::runtime_error("socket is not initialized");
    if (!_tools.socket->handle())
        throw std::runtime_error("socket is not connected");

    _tools.socket->set(zmq::sockopt::sndtimeo, timeout);
    _tools.socket->set(zmq::sockopt::rcvtimeo, timeout);

    auto sent{ _tools.socket->send(zmq::buffer(msg), zmq::send_flags::none) };
    if (!sent)
        throw std::runtime_error("send timeout");
    
    message_t recv_msg;
    auto res{ _tools.socket->recv(recv_msg, zmq::recv_flags::none) };
    if (!res)
        throw std::runtime_error("recv timeout");
    return recv_msg;
}