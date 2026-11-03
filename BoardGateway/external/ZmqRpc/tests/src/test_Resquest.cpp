#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include <string>
#include <type_traits>
#include "zmq_rpc/controller/request.hpp"

using str_view = std::string_view;

using zmq_rpc::controller::Request;
using json = zmq_rpc::json::json;

struct Data {
    double v1{ 0.0 };
    int v2{ 0 };
};

static void to_json(json& j, const Data& data) {
    zmq_rpc::json::add_to_json(j, "v1", data.v1);
    zmq_rpc::json::add_to_json(j, "v2", data.v2);
}

static void from_json(const json& j, Data& data) {
    zmq_rpc::json::get_from_json_to(j, "v1", data.v1);
    zmq_rpc::json::get_from_json_to(j, "v2", data.v2);
}

static bool operator==(const Data& lhs, const Data& rhs) {
    return lhs.v1 == rhs.v1 && lhs.v2 == rhs.v2;
}

static constexpr str_view path{ "path" };
static constexpr str_view method{ "method" };

static std::string make_request_json() {
    auto req{ zmq_rpc::controller::make_request(path, method) };
    zmq_rpc::json::json j;
    zmq_rpc::controller::to_json(j, req);
    return j.dump();
}

template<typename T>
static std::string make_request_json(T&& data) {
    auto req{ zmq_rpc::controller::make_request(path, method, std::forward<T>(data)) };
    zmq_rpc::json::json j;
    zmq_rpc::controller::to_json(j, req);
    return j.dump();
}

TEST(TestRequest, ConstructRequestVoid) {
    auto req{ zmq_rpc::controller::make_request(path, method) };
    ASSERT_EQ(req.api.path, path);
    ASSERT_EQ(req.api.method, method);
}

TEST(TestRequest, ConstructRequestWithData) {
    Data data{ 10.5, 20 };
    auto req{ zmq_rpc::controller::make_request(path, method, data) };
    static_assert(std::is_same_v<decltype(req)::data_t, Data>);
    ASSERT_EQ(req.api.path, path);
    ASSERT_EQ(req.api.method, method);
    ASSERT_EQ(req.data, data);
}

TEST(TestRequest, CheckRequestVoidToJson) {
    auto req{ zmq_rpc::controller::make_request(path, method) };
    zmq_rpc::json::json j = req;
    ASSERT_TRUE(j.contains("path"));
    ASSERT_TRUE(j.contains("method"));
}

TEST(TestRequest, CheckRequestWithDataToJson) {
    Data data{ 10.5, 20 };
    auto req{ zmq_rpc::controller::make_request(path, method, data) };
    zmq_rpc::json::json j = req;
    ASSERT_TRUE(j.contains("path"));
    ASSERT_TRUE(j.contains("method"));
    ASSERT_TRUE(j.contains("data"));
}

TEST(TestRequest, CheckGetRequestVoidFromJson) {
    auto json_str{ make_request_json() };
    auto j = json::parse(json_str);
    auto req{ j.get<Request<void>>() };
    ASSERT_EQ(req.api.path, path);
    ASSERT_EQ(req.api.method, method);
}

TEST(TestRequest, CheckGetRequestWithDataFromJson) {
    Data data{ 10.5, 20 };
    auto json_str{ make_request_json(data) };
    auto j = json::parse(json_str);
    auto req{ j.get<Request<Data>>() };
    ASSERT_EQ(req.api.path, path);
    ASSERT_EQ(req.api.method, method);
    ASSERT_EQ(req.data, data);
}

TEST(TestRequest, CheckGetRequestVoidFromJsonThatIsRequestWithData) {
    Data data{ 10.5, 20 };
    auto json_str{ make_request_json(data) };
    auto j = json::parse(json_str);
    auto req{ j.get<Request<void>>() };
    ASSERT_EQ(req.api.path, path);
    ASSERT_EQ(req.api.method, method);
}

TEST(TestRequest, CheckGetRequestWithDataFromJsonThatIsRequestVoid) {
    Data data{};
    auto json_str{ make_request_json() };
    auto j = json::parse(json_str);
    auto req{ j.get<Request<Data>>() };
    ASSERT_EQ(req.api.path, path);
    ASSERT_EQ(req.api.method, method);
    ASSERT_EQ(req.data, data);
}