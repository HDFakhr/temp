#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include "zmq_rpc/client_tools.hpp"
#include "zmq_rpc/client_pipeline.hpp"
#include "zmq_rpc/server_tools.hpp"
#include "zmq_rpc/server_pipeline.hpp"

using zmq_rpc::ClientTools;
using zmq_rpc::ClientPipeline;
using zmq_rpc::ServerTools;
using zmq_rpc::ServerPipeline;

TEST(TestServerAndClient, Test) {
    zmq_rpc::context_t context{1};
    using mock_api_t = testing::MockFunction<int(int)>;

    auto data{ 10 };
    auto result{ 20 };
    std::string url{ "inproc://test" };
    std::string api_path{ "/api/test" };
    std::string api_method{ "GET" };

    mock_api_t api;
    EXPECT_CALL(api, Call(data)).WillOnce(testing::Return(result));
    ServerTools server_tools;
    ServerPipeline(server_tools)
        .initial(context, url)
        .initial_thread(100)
        .register_api(api_path, api_method, api.AsStdFunction())
        .start();
    
    ClientTools client_tools;
    auto resp{ ClientPipeline(client_tools)
        .initial(context, url)
        .send<int>(api_path, api_method, data)
    };
    ASSERT_TRUE(resp.info.status);
    ASSERT_TRUE(resp.info.error.empty());
    ASSERT_EQ(resp.result, result);
}