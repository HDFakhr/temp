#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include <string>
#include <span>
#include <glog/logging.h>
#include "zmq_rpc/controller/controller.hpp"

using str_view = std::string_view;
using byte_t = char;

using json = zmq_rpc::json::json;
using zmq_rpc::controller::Request;
using zmq_rpc::controller::Response;
using zmq_rpc::controller::Controller;

struct FakeApi {
    using str = std::string;
    using str_view = std::string_view;
    using data_t = int;
    using result_t = int;

    static constexpr str_view path{ "/api/first" };
    static constexpr str_view method{ "GET" };
    static constexpr data_t data{ 10 };
    static constexpr result_t result{ 20 };

    static str make_request(data_t d) {
        json j = zmq_rpc::controller::make_request(path, method, d);
        return j.dump();
    }

    static str make_request() {
        json j = zmq_rpc::controller::make_request(path, method);
        return j.dump();
    }
};

class TestController : public testing::Test {
public:

    template<typename R, typename T>
    auto route_request(T&& data) -> Response<R> {
        using api_t = FakeApi;
        auto req{ api_t::make_request(data) };
        auto resp_str{ controller.route(std::span<const char>(req.data(), req.size())) };
        auto resp_j = json::parse(resp_str);
        return resp_j.get<Response<R>>(); 
    }

    template<typename R>
    auto route_request() -> Response<R> {
        using api_t = FakeApi;
        auto req{ api_t::make_request() };
        auto resp_str{ controller.route(std::span<const char>(req.data(), req.size())) };
        auto resp_j = json::parse(resp_str);
        return resp_j.get<Response<R>>(); 
    }

protected:
    Controller controller;
};

TEST_F(TestController, CheckControllerWithRequestDataAndResponseResult) {
    using api_t = FakeApi;
    using data_t = api_t::data_t;
    using mock_handler_t = testing::MockFunction<api_t::result_t(api_t::data_t)>;

    mock_handler_t mock_handler;
    EXPECT_CALL(mock_handler, Call(api_t::data)).WillOnce(testing::Return(api_t::result));
    controller.register_api(api_t::path, api_t::method, mock_handler.AsStdFunction());
    auto resp{ route_request<api_t::result_t>(api_t::data) };
    ASSERT_TRUE(resp.info.status);
    ASSERT_EQ(resp.result, api_t::result);
    ASSERT_TRUE(resp.info.error.empty());
}

TEST_F(TestController, CheckControllerWithResponseResult) {
    using api_t = FakeApi;
    using data_t = api_t::data_t;
    using mock_handler_t = testing::MockFunction<api_t::result_t()>;

    mock_handler_t mock_handler;
    EXPECT_CALL(mock_handler, Call()).WillOnce(testing::Return(api_t::result));
    controller.register_api(api_t::path, api_t::method, mock_handler.AsStdFunction());
    auto resp{ route_request<api_t::result_t>() };
    ASSERT_TRUE(resp.info.status);
    ASSERT_EQ(resp.result, api_t::result);
    ASSERT_TRUE(resp.info.error.empty());
}

TEST_F(TestController, CheckControllerWithRequestData) {
    using api_t = FakeApi;
    using data_t = api_t::data_t;
    using mock_handler_t = testing::MockFunction<void(api_t::data_t)>;

    mock_handler_t mock_handler;
    EXPECT_CALL(mock_handler, Call(api_t::data)).Times(1);
    controller.register_api(api_t::path, api_t::method, mock_handler.AsStdFunction());
    auto resp{ route_request<void>(api_t::data) };
    ASSERT_TRUE(resp.info.status);
    ASSERT_TRUE(resp.info.error.empty());
}

TEST_F(TestController, CheckControllerWithNoRequestDataAndNoResponseResult) {
    using api_t = FakeApi;
    using data_t = api_t::data_t;
    using mock_handler_t = testing::MockFunction<void()>;

    mock_handler_t mock_handler;
    EXPECT_CALL(mock_handler, Call()).Times(1);
    controller.register_api(api_t::path, api_t::method, mock_handler.AsStdFunction());
    auto resp{ route_request<void>() };
    ASSERT_TRUE(resp.info.status);
    ASSERT_TRUE(resp.info.error.empty());
}

TEST_F(TestController, CheckControllerForApiNotFound) {
    using api_t = FakeApi;
    using data_t = api_t::data_t;

    auto resp{ route_request<void>() };
    ASSERT_FALSE(resp.info.status);
    ASSERT_FALSE(resp.info.error.empty());
}