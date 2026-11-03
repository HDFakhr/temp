#include <gtest/gtest.h>
#include "board_zmq_client/pipeline.hpp"
#include "board_zmq_client/api/capture/status/handler.hpp"
#include "board_zmq_client/api/capture/device/info/handler.hpp"
#include <iostream>

namespace nzc = nooyan::zmq_client;

using nzc::Tools;

std::ostream& operator<<(std::ostream& stream, const nzc::api::capture::status::Get::Result& result) {
    stream << "[Capture Status]\n"
        << "id: " << result.id << '\n'
        << "adc: " << result.adc << '\n'
        << "channel_num: " << result.channel_num << '\n'
        << "size: " << result.size << '\n'
        << "freq: " << result.freq << '\n'
        << "fs: " << result.fs << '\n'
        << "filename: " << result.filename << '\n'
        << "block_size: " << result.block_size << '\n'
        << "error: " << result.error << '\n'
        << "captured_size: " << result.captured_size << '\n'
        << "drop_size: " << result.drop_size << '\n'
        << "dma_capacity: " << result.dma_capacity << '\n'
        << "dma_size: " << result.dma_size << '\n'
        << "rate: " << result.rate << '\n'
        << "progress: " << result.progress << '\n'
        << std::boolalpha
        << "is_wb: " << result.is_wb << '\n'
        << "is_run: " << result.is_run
        << std::dec;
    return stream;
}

std::ostream& operator<<(std::ostream& stream, const nzc::api::capture::device::info::Get::Info& info) {
    stream << "[Device Info]\n"
        << "name: " << info.name << '\n'
        << "addr: " << info.addr << '\n'
        << "index: " << info.index << '\n'
        << "total space: " << info.total_space << '\n'
        << "free space: " << info.free_space << '\n'
        << "block size: " << info.block_size;
    return stream;
}

TEST(TestCaptureApis, CheckCaptureStatus) {
    auto timeout = 1000;
    Tools tools;
    nzc::Pipeline(tools)
        .initial_by_json_file(std::filesystem::path(BOARD_CONFIG_FILE));

    try {
        auto resp = nzc::api::capture::status::Handler(tools.capture)
            .get(timeout);
        std::cout << resp << '\n';
    } catch (const std::runtime_error& e) {
        std::cout << "Error: " << e.what() << '\n';
    }
}

TEST(TestCaptureApis, CheckDeviceInfo) {
    auto timeout = 1000;
    Tools tools;
    nzc::Pipeline(tools)
        .initial_by_json_file(std::filesystem::path(BOARD_CONFIG_FILE));

    try {
        auto resp = nzc::api::capture::device::info::Handler(tools.capture)
            .get(timeout);
        for (const auto& info : resp)
            std::cout << info << '\n';
    } catch (const std::runtime_error& e) {
        std::cout << "Error: " << e.what() << '\n';
    }
}