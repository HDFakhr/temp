#include <gtest/gtest.h>
#include "board_zmq_client/pipeline.hpp"
// #include "board_zmq_client/spectrum/pipeline.hpp"
#include "board_zmq_client/api/spectrum/handler.hpp"
#include <fmt/format.h>

namespace nzc = nooyan::zmq_client;

using nzc::Tools;

static void test_spectrum_channel(int channel, int timeout) {
    Tools tools;
    std::cout << "Channel: " << channel << '\n';
    SCOPED_TRACE(fmt::format("Check spectrum for channel{}", channel));
    nzc::Pipeline(tools)
        .initial_by_json_file(std::filesystem::path(BOARD_CONFIG_FILE));
    try {
        auto spectrum = nzc::api::spectrum::Handler(tools.spectrums[channel])
            .get(timeout);
        std::cout << "Start: " << spectrum.start << '\n';
        std::cout << "Step: " << spectrum.step << '\n';
        std::cout << "Length: " << spectrum.data.size() << '\n';
    } catch (const std::runtime_error& e) {
        std::cout << "Error: " << e.what() << '\n';
    }
    std::cout << '\n';
}

TEST(TestSpectrumApis, GetSpectrumWhenSocketIsNotInitializedThenThrowAnError)
{
    Tools tools;

    int timeout = 1000;
    try {
        nzc::api::spectrum::Handler(tools.spectrums[0])
            .get(timeout);
        FAIL() << "should throw an error!";
    } catch (const std::runtime_error& e) {
        SUCCEED();
    }
}

TEST(TestSpectrumApis, GetSpectrumWhenSocketIsInitializedThenCheckResult)
{
    int timeout = 1000;
    
    for (int channel = 0; channel < Tools::SPECTRUM_CHANNELS; ++channel)
        test_spectrum_channel(channel, timeout);
}