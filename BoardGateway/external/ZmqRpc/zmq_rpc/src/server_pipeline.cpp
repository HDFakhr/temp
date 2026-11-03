#include "zmq_rpc/server_pipeline.hpp"
#include "zmq_rpc/server/pipeline.hpp"

using zmq_rpc::ServerPipeline;
using zmq_rpc::server::Pipeline;

ServerPipeline& ServerPipeline::initial(context_t& ctx, const str& url) {
    using byte_t = char;

    _tools.server.data_handler = [tools = std::ref(_tools)](std::span<const char> msg) -> str {
        return tools.get().controller.route(msg);
    };
    Pipeline(_tools.server).initial(ctx, url);

    return *this;
}

ServerPipeline& ServerPipeline::initial_thread(int timeout) {
    Pipeline(_tools.server).initial_thread(timeout);
    return *this;
}

ServerPipeline& ServerPipeline::start() {
    Pipeline(_tools.server).start();
    return *this;
}

ServerPipeline& ServerPipeline::stop() {
    Pipeline(_tools.server).stop();
    return *this;
}