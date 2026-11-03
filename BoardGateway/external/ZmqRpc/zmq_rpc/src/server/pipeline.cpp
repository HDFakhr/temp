#include "zmq_rpc/server/pipeline.hpp"
#include "zmq_rpc/thread/builder.hpp"

using zmq_rpc::server::Pipeline;

Pipeline& Pipeline::initial(context_t& ctx, const str& url) {
    _tools.socket = std::make_shared<tools_t::socket_t>(ctx, tools_t::socket_type);
    _tools.socket->bind(url);
    return *this;
}

Pipeline& Pipeline::initial_thread(int timeout) {
    using message_t = zmq::message_t;
    using byte_t = tools_t::byte_t;

    if (!_tools.socket)
        return *this;
    
    _tools.socket->set(zmq::sockopt::sndtimeo, timeout);
    _tools.socket->set(zmq::sockopt::rcvtimeo, timeout);

    auto worker = [tools = std::ref(_tools)]() {
        message_t recv_msg;
        auto res = tools.get().socket->recv(recv_msg, zmq::recv_flags::none);
        if (res) {
            auto resp{ tools.get().data_handler(std::span(reinterpret_cast<const byte_t*>(recv_msg.data()), recv_msg.size())) };
            message_t resp_msg{ std::move(resp) };
            tools.get().socket->send(resp_msg, zmq::send_flags::none);
        }
    };

    _tools.thread = thread::ThreadBuilder::build(std::move(worker));
    return *this;
}

Pipeline& Pipeline::start() {
    using message_t = zmq::message_t;
    using byte_t = tools_t::byte_t;

    if (!_tools.thread.start)
        return *this;
    
    _tools.thread.start();
    return *this;
}

Pipeline& Pipeline::stop() {
    if (!_tools.thread.stop)
        return *this;
    
    _tools.thread.stop();
    return *this;
}