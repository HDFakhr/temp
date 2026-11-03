#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include "zmq_rpc/controller/api.hpp"

using zmq_rpc::controller::Api;

TEST(TestApi, CheckJson) {
    Api api;
    api.path = "/test/path";
    api.method = "GET";

    zmq_rpc::json::json j;
    j["path"] = api.path;
    j["method"] = api.method;

    auto j_str = j.dump();
    auto j_span = std::span<const char>(j_str.data(), j_str.size());
    auto j_parsed = zmq_rpc::json::json::parse(j_span.begin(), j_span.end());

    auto api_parsed{ j_parsed.get<Api>()};
    EXPECT_EQ(api_parsed.path, api.path);
    EXPECT_EQ(api_parsed.method, api.method);
}