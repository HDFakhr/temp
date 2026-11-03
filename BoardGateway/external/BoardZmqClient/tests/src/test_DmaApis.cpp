#include <gtest/gtest.h>
#include "board_zmq_client/pipeline.hpp"
#include "board_zmq_client/api/dma/status/handler.hpp"
#include "board_zmq_client/api/dma/start/handler.hpp"
#include "board_zmq_client/api/dma/data/handler.hpp"
#include "board_zmq_client/utils/dma_channel_num.hpp"
#include <fmt/format.h>

namespace nzc = nooyan::zmq_client;

using nzc::Tools;
using ChannelType = nzc::utils::Dma::ChannelType;
using ChannelNum = nzc::utils::Dma::ChannelNum;

template<ChannelType T, int N, int ADC>
static constexpr auto channel_num_v = nzc::utils::Dma::ChannelNum<T, N, ADC>::value;

template<ChannelType T, int N, int ADC>
struct ChannelInfo {
    static constexpr auto name = "";
};

template<>
struct ChannelInfo<ChannelType::WB, 0, 0> {
    static constexpr auto name = "ADC=0,WB=0";
};

template<>
struct ChannelInfo<ChannelType::NB, 0, 0> {
    static constexpr auto name = "ADC=0,NB=0";
};

template<>
struct ChannelInfo<ChannelType::NB, 1, 0> {
    static constexpr auto name = "ADC=0,NB=1";
};

template<>
struct ChannelInfo<ChannelType::NB, 2, 0> {
    static constexpr auto name = "ADC=0,NB=2";
};

template<>
struct ChannelInfo<ChannelType::NB, 3, 0> {
    static constexpr auto name = "ADC=0,NB=3";
};

template<>
struct ChannelInfo<ChannelType::WB, 0, 1> {
    static constexpr auto name = "ADC=1,WB=0";
};

template<>
struct ChannelInfo<ChannelType::NB, 0, 1> {
    static constexpr auto name = "ADC=1,NB=0";
};

template<>
struct ChannelInfo<ChannelType::NB, 1, 1> {
    static constexpr auto name = "ADC=1,NB=1";
};

template<>
struct ChannelInfo<ChannelType::NB, 2, 1> {
    static constexpr auto name = "ADC=1,NB=2";
};

template<>
struct ChannelInfo<ChannelType::NB, 3, 1> {
    static constexpr auto name = "ADC=1,NB=3";
};

template<ChannelType T, int N, int ADC>
static void test_dma_status(int timeout) {
    static constexpr auto channel{ channel_num_v<T, N, ADC> };
    static constexpr auto channel_name{ ChannelInfo<T, N, ADC>::name };
    Tools tools;
    SCOPED_TRACE(fmt::format("Check dma status for {}", channel_name));
    nzc::Pipeline(tools)
        .initial_by_json_file(std::filesystem::path(BOARD_CONFIG_FILE));
    try {
        auto status = nzc::api::dma::status::Handler(tools.dma[channel])
            .get(timeout);
        std::cout << "Size: " << status.size << '\n';
        std::cout << "Bulk Size: " << status.bulk_size << '\n';
        std::cout << "Max Size: " << status.max_size << '\n';
    } catch (const std::runtime_error& e) {
        std::cout << "Error: " << e.what() << '\n';
    }
    std::cout << '\n';
}

template<ChannelType T, int N, int ADC>
static void test_dma_start(int timeout) {
    static constexpr auto channel{ channel_num_v<T, N, ADC> };
    static constexpr auto channel_name{ ChannelInfo<T, N, ADC>::name };
    Tools tools;
    SCOPED_TRACE(fmt::format("Check dma start for {}", channel_name));
    nzc::Pipeline(tools)
        .initial_by_json_file(std::filesystem::path(BOARD_CONFIG_FILE));
    try {
        nzc::api::dma::start::Post::Data data;
        data.size = 4 * 1024 * 1024;
        auto resp = nzc::api::dma::start::Handler(tools.dma[channel])
            .post(data, timeout);
        std::cout << "Size: " << resp.size << '\n';
        std::cout << "Max Size: " << resp.max_size << '\n';
    } catch (const std::runtime_error& e) {
        std::cout << "Error: " << e.what() << '\n';
    }
    std::cout << '\n';
}

template<ChannelType T, int N, int ADC>
static void test_dma_data(int timeout) {
    static constexpr auto channel{ channel_num_v<T, N, ADC> };
    static constexpr auto channel_name{ ChannelInfo<T, N, ADC>::name };
    Tools tools;
    SCOPED_TRACE(fmt::format("Check dma start for {}", channel_name));
    nzc::Pipeline(tools)
        .initial_by_json_file(std::filesystem::path(BOARD_CONFIG_FILE));
    try {
        nzc::api::dma::data::Get::Data data;
        data.offset = 0;
        data.chunk_size = 1024 * 1024;
        auto resp = nzc::api::dma::data::Handler(tools.dma[channel])
            .data(data, timeout);
        std::cout << "Data size: " << resp.size() * sizeof(decltype(resp)::value_type) << '\n';
    } catch (const std::runtime_error& e) {
        std::cout << "Error: " << e.what() << '\n';
    }
    std::cout << '\n';
}

TEST(TestDmaApis, CheckDmaStatusWhenSocketIsNotInitializedThenThrowAnError)
{
    Tools tools;

    int timeout = 1000;
    try {
        nzc::api::dma::status::Handler(tools.dma[0])
            .get(timeout);
        FAIL() << "should throw an error!";
    } catch (const std::runtime_error& e) {
        SUCCEED();
    }
}

TEST(TestDmaApis, CheckDmaStatusWhenSocketIsInitializedThenCheckResult)
{
    int timeout = 1000;
    test_dma_status<ChannelType::WB, 0, 0>(timeout);
    test_dma_status<ChannelType::NB, 0, 0>(timeout);
    test_dma_status<ChannelType::NB, 1, 0>(timeout);
    test_dma_status<ChannelType::NB, 2, 0>(timeout);
    test_dma_status<ChannelType::NB, 3, 0>(timeout);

    test_dma_status<ChannelType::WB, 0, 1>(timeout);
    test_dma_status<ChannelType::NB, 0, 1>(timeout);
    test_dma_status<ChannelType::NB, 1, 1>(timeout);
    test_dma_status<ChannelType::NB, 2, 1>(timeout);
    test_dma_status<ChannelType::NB, 3, 1>(timeout);
}

TEST(TestDmaApis, CheckDmaStartWhenSocketIsInitializedThenCheckResult)
{
    int timeout = 1000;
    test_dma_start<ChannelType::WB, 0, 0>(timeout);
    test_dma_start<ChannelType::NB, 0, 0>(timeout);
    test_dma_start<ChannelType::NB, 1, 0>(timeout);
    test_dma_start<ChannelType::NB, 2, 0>(timeout);
    test_dma_start<ChannelType::NB, 3, 0>(timeout);

    test_dma_start<ChannelType::WB, 0, 1>(timeout);
    test_dma_start<ChannelType::NB, 0, 1>(timeout);
    test_dma_start<ChannelType::NB, 1, 1>(timeout);
    test_dma_start<ChannelType::NB, 2, 1>(timeout);
    test_dma_start<ChannelType::NB, 3, 1>(timeout);
}

TEST(TestDmaApis, CheckDmaDataWhenSocketIsInitializedThenCheckResult)
{
    int timeout = 10000;
    test_dma_data<ChannelType::WB, 0, 0>(timeout);
    test_dma_data<ChannelType::NB, 0, 0>(timeout);
    test_dma_data<ChannelType::NB, 1, 0>(timeout);
    test_dma_data<ChannelType::NB, 2, 0>(timeout);
    test_dma_data<ChannelType::NB, 3, 0>(timeout);

    test_dma_data<ChannelType::WB, 0, 1>(timeout);
    test_dma_data<ChannelType::NB, 0, 1>(timeout);
    test_dma_data<ChannelType::NB, 1, 1>(timeout);
    test_dma_data<ChannelType::NB, 2, 1>(timeout);
    test_dma_data<ChannelType::NB, 3, 1>(timeout);
}