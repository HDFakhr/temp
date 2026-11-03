#include "zmq_rpc/client_pipeline.hpp"

using zmq_rpc::ClientPipeline;

ClientPipeline& ClientPipeline::initial(context_t& ctx, const str& url) {
    client::Pipeline(_tools).initial(ctx, url);
    return *this;
}