#include <gtest/gtest.h>
#include "board_zmq_client/pipeline.hpp"
#include "board_zmq_client/api/board/ok/handler.hpp"
#include "board_zmq_client/api/board/adc/channel/user/handler.hpp"
#include "board_zmq_client/api/board/adc/handler.hpp"
#include "board_zmq_client/api/board/channel/adc/handler.hpp"

namespace nzc = nooyan::zmq_client;

using nzc::Tools;

TEST(TestBoardApis, SetAdcByUserWhenSocketIsNotInitializedThenThrowAnError)
{
    using handler_t = nzc::api::board::adc::channel::user::Handler;
    Tools tools;
    int timeout = 5000;

    handler_t::post_t::Data data;
    try {
        handler_t(tools.board).post(data, timeout);
        FAIL() << "should throw an error!";
    } catch (const std::runtime_error& e) {
        SUCCEED();
    }
}

TEST(TestBoardApis, CheckBoardIsOk) {
    using handler_t = nzc::api::board::ok::Handler;

    Tools tools;
    int timeout = 1000;

    nzc::Pipeline(tools)
        .initial_by_json_file(std::filesystem::path(BOARD_CONFIG_FILE));
    {
        SCOPED_TRACE("Check api");
        try {
            auto resp = handler_t(tools.board).get(timeout);
            ASSERT_EQ(resp, std::string("ok"));
        } catch (const std::runtime_error& e) {
            FAIL() << "Error: " << e.what();
        }
    }
}


TEST(TestBoardApis, SetAdcWhenSocketIsInitializedThenCheckResult) {
    using handler_t = nzc::api::board::adc::Handler;
    using data_t = handler_t::post_t::Data;

    Tools tools;
    int timeout = 5000;

    nzc::Pipeline(tools)
        .initial_by_json_file(std::filesystem::path(BOARD_CONFIG_FILE));
    {
        data_t data;
        data.adc = 0;
        data.adc_prec = 8;
        SCOPED_TRACE("Check api when set adc prec");
        try {
            auto resp = handler_t(tools.board).post(data, timeout);
            ASSERT_EQ(resp.adc, data.adc);
            ASSERT_EQ(resp.adc_prec, data.adc_prec);
        } catch (const std::runtime_error& e) {
            FAIL() << "Error: " << e.what();
        }
    }
}

TEST(TestBoardApis, SetAdcByUserWhenSocketIsInitializedThenCheckResult) {
    using handler_t = nzc::api::board::adc::channel::user::Handler;
    using data_t = handler_t::post_t::Data;
    using channel_t = handler_t::post_t::Channel;

    Tools tools;
    int timeout = 5000;

    nzc::Pipeline(tools)
        .initial_by_json_file(std::filesystem::path(BOARD_CONFIG_FILE));
    
    {
        data_t data;
        SCOPED_TRACE("Check api when data.adc is not set");
        try {
            handler_t(tools.board).post(data, timeout);
            FAIL() << "should throw an error!";
        } catch (const std::runtime_error& e) {
            GTEST_LOG_(INFO) << "Error: " << e.what();
        }
    }
    {
        data_t data;
        data.adc = 0;
        SCOPED_TRACE("Check api when data.freq is not set");
        try {
            handler_t(tools.board).post(data, timeout);
            FAIL() << "should throw an error!";
        } catch (const std::runtime_error& e) {
            GTEST_LOG_(INFO) << "Error: " << e.what();
        }
    }
    {
        data_t data;
        data.adc = 0;
        data.freq = 1300000000;
        SCOPED_TRACE("Check api when data has minimum requirements for adc 0 wb");
        try {
            auto resp = handler_t(tools.board).post(data, timeout);
            ASSERT_EQ(resp.adc, data.adc);
            ASSERT_EQ(resp.channel, std::string(channel_t::wb));
            ASSERT_EQ(resp.channel_num, 0);
            ASSERT_EQ(resp.user_freq, data.freq);
            ASSERT_FALSE(resp.agc_enable);
            ASSERT_FALSE(resp.with_fractional_resampler);
            ASSERT_FALSE(resp.with_matched_filter);
            ASSERT_FALSE(resp.with_symbol_output);
        } catch (const std::runtime_error& e) {
            FAIL() << "Error: " << e.what();
        }
    }
    {
        data_t data;
        data.adc = 0;
        data.channel = channel_t::nb;
        data.channel_num = 0;
        data.freq = 1300000000;
        data.sampling_rate = 4000000;
        SCOPED_TRACE("Check api when data has minimum requirements for adc 0 nb 0");
        try {
            auto resp = handler_t(tools.board).post(data, timeout);
            ASSERT_EQ(resp.adc, data.adc);
            ASSERT_EQ(resp.channel, std::string(channel_t::nb));
            ASSERT_EQ(resp.channel_num, 0);
            ASSERT_EQ(resp.user_freq, data.freq);
            ASSERT_FALSE(resp.with_fractional_resampler);
            ASSERT_FALSE(resp.with_matched_filter);
            ASSERT_FALSE(resp.with_symbol_output);
        } catch (const std::runtime_error& e) {
            FAIL() << "Error: " << e.what();
        }
    }
    {
        data_t data;
        data.adc = 0;
        data.channel = channel_t::nb;
        data.channel_num = 1;
        data.freq = 1300000000;
        data.sampling_rate = 4000000;
        SCOPED_TRACE("Check api when data has minimum requirements for adc 0 nb 1");
        try {
            auto resp = handler_t(tools.board).post(data, timeout);
            ASSERT_EQ(resp.adc, data.adc);
            ASSERT_EQ(resp.channel, std::string(channel_t::nb));
            ASSERT_EQ(resp.channel_num, 1);
            ASSERT_EQ(resp.user_freq, data.freq);
            ASSERT_FALSE(resp.with_fractional_resampler);
            ASSERT_FALSE(resp.with_matched_filter);
            ASSERT_FALSE(resp.with_symbol_output);
        } catch (const std::runtime_error& e) {
            FAIL() << "Error: " << e.what();
        }
    }
    {
        data_t data;
        data.adc = 0;
        data.channel = channel_t::nb;
        data.channel_num = 2;
        data.freq = 1300000000;
        data.sampling_rate = 4000000;
        SCOPED_TRACE("Check api when data has minimum requirements for adc 0 nb 2");
        try {
            auto resp = handler_t(tools.board).post(data, timeout);
            ASSERT_EQ(resp.adc, data.adc);
            ASSERT_EQ(resp.channel, std::string(channel_t::nb));
            ASSERT_EQ(resp.channel_num, 2);
            ASSERT_EQ(resp.user_freq, data.freq);
            ASSERT_FALSE(resp.with_fractional_resampler);
            ASSERT_FALSE(resp.with_matched_filter);
            ASSERT_FALSE(resp.with_symbol_output);
        } catch (const std::runtime_error& e) {
            FAIL() << "Error: " << e.what();
        }
    }
    {
        data_t data;
        data.adc = 0;
        data.channel = channel_t::nb;
        data.channel_num = 3;
        data.freq = 1300000000;
        data.sampling_rate = 4000000;
        SCOPED_TRACE("Check api when data has minimum requirements for adc 0 nb 3");
        try {
            auto resp = handler_t(tools.board).post(data, timeout);
            ASSERT_EQ(resp.adc, data.adc);
            ASSERT_EQ(resp.channel, std::string(channel_t::nb));
            ASSERT_EQ(resp.channel_num, 3);
            ASSERT_EQ(resp.user_freq, data.freq);
            ASSERT_FALSE(resp.with_fractional_resampler);
            ASSERT_FALSE(resp.with_matched_filter);
            ASSERT_FALSE(resp.with_symbol_output);
        } catch (const std::runtime_error& e) {
            FAIL() << "Error: " << e.what();
        }
    }
    {
        data_t data;
        data.adc = 1;
        data.freq = 1300000000;
        SCOPED_TRACE("Check api when data has minimum requirements for adc 1 wb");
        try {
            auto resp = handler_t(tools.board).post(data, timeout);
            ASSERT_EQ(resp.adc, data.adc);
            ASSERT_EQ(resp.channel, std::string(channel_t::wb));
            ASSERT_EQ(resp.channel_num, 0);
            ASSERT_EQ(resp.user_freq, data.freq);
            ASSERT_FALSE(resp.agc_enable);
            ASSERT_FALSE(resp.with_fractional_resampler);
            ASSERT_FALSE(resp.with_matched_filter);
            ASSERT_FALSE(resp.with_symbol_output);
        } catch (const std::runtime_error& e) {
            FAIL() << "Error: " << e.what();
        }
    }
    {
        data_t data;
        data.adc = 1;
        data.channel = channel_t::nb;
        data.channel_num = 0;
        data.freq = 1300000000;
        data.sampling_rate = 4000000;
        SCOPED_TRACE("Check api when data has minimum requirements for adc 1 nb 0");
        try {
            auto resp = handler_t(tools.board).post(data, timeout);
            ASSERT_EQ(resp.adc, data.adc);
            ASSERT_EQ(resp.channel, std::string(channel_t::nb));
            ASSERT_EQ(resp.channel_num, 0);
            ASSERT_EQ(resp.user_freq, data.freq);
            ASSERT_FALSE(resp.with_fractional_resampler);
            ASSERT_FALSE(resp.with_matched_filter);
            ASSERT_FALSE(resp.with_symbol_output);
        } catch (const std::runtime_error& e) {
            FAIL() << "Error: " << e.what();
        }
    }
    {
        data_t data;
        data.adc = 1;
        data.channel = channel_t::nb;
        data.channel_num = 1;
        data.freq = 1300000000;
        data.sampling_rate = 4000000;
        SCOPED_TRACE("Check api when data has minimum requirements for adc 1 nb 1");
        try {
            auto resp = handler_t(tools.board).post(data, timeout);
            ASSERT_EQ(resp.adc, data.adc);
            ASSERT_EQ(resp.channel, std::string(channel_t::nb));
            ASSERT_EQ(resp.channel_num, 1);
            ASSERT_EQ(resp.user_freq, data.freq);
            ASSERT_FALSE(resp.with_fractional_resampler);
            ASSERT_FALSE(resp.with_matched_filter);
            ASSERT_FALSE(resp.with_symbol_output);
        } catch (const std::runtime_error& e) {
            FAIL() << "Error: " << e.what();
        }
    }
    {
        data_t data;
        data.adc = 1;
        data.channel = channel_t::nb;
        data.channel_num = 2;
        data.freq = 1300000000;
        data.sampling_rate = 4000000;
        SCOPED_TRACE("Check api when data has minimum requirements for adc 1 nb 2");
        try {
            auto resp = handler_t(tools.board).post(data, timeout);
            ASSERT_EQ(resp.adc, data.adc);
            ASSERT_EQ(resp.channel, std::string(channel_t::nb));
            ASSERT_EQ(resp.channel_num, 2);
            ASSERT_EQ(resp.user_freq, data.freq);
            ASSERT_FALSE(resp.with_fractional_resampler);
            ASSERT_FALSE(resp.with_matched_filter);
            ASSERT_FALSE(resp.with_symbol_output);
        } catch (const std::runtime_error& e) {
            FAIL() << "Error: " << e.what();
        }
    }
    {
        data_t data;
        data.adc = 1;
        data.channel = channel_t::nb;
        data.channel_num = 3;
        data.freq = 1300000000;
        data.sampling_rate = 4000000;
        SCOPED_TRACE("Check api when data has minimum requirements for adc 1 nb 3");
        try {
            auto resp = handler_t(tools.board).post(data, timeout);
            ASSERT_EQ(resp.adc, data.adc);
            ASSERT_EQ(resp.channel, std::string(channel_t::nb));
            ASSERT_EQ(resp.channel_num, 3);
            ASSERT_EQ(resp.user_freq, data.freq);
            ASSERT_FALSE(resp.with_fractional_resampler);
            ASSERT_FALSE(resp.with_matched_filter);
            ASSERT_FALSE(resp.with_symbol_output);
        } catch (const std::runtime_error& e) {
            FAIL() << "Error: " << e.what();
        }
    }
}

TEST(TestBoardApis, SetADCForChannelWhenSocketIsInitializedThenCheckResult) {
    using handler_t = nzc::api::board::channel::adc::Handler;
    using data_t = handler_t::post_t::Data;

    Tools tools;
    int timeout = 5000;

    nzc::Pipeline(tools)
        .initial_by_json_file(std::filesystem::path(BOARD_CONFIG_FILE));
    
    {
        SCOPED_TRACE("Check select adc 0 for channel 0");
        data_t data;
        data.channel = 0;
        data.adc = 0;
        try {
            auto resp = handler_t(tools.board).post(data, timeout);
            ASSERT_EQ(resp.channel, data.channel);
            ASSERT_EQ(resp.adc, data.adc);
        } catch (const std::runtime_error& e) {
            FAIL() << "Error: " << e.what();
        }
    }
    {
        SCOPED_TRACE("Check select adc 1 for channel 0");
        data_t data;
        data.channel = 0;
        data.adc = 1;
        try {
            auto resp = handler_t(tools.board).post(data, timeout);
            ASSERT_EQ(resp.channel, data.channel);
            ASSERT_EQ(resp.adc, data.adc);
        } catch (const std::runtime_error& e) {
            FAIL() << "Error: " << e.what();
        }
    }
    {
        SCOPED_TRACE("Check select adc 0 for channel 1");
        data_t data;
        data.channel = 1;
        data.adc = 0;
        try {
            auto resp = handler_t(tools.board).post(data, timeout);
            ASSERT_EQ(resp.channel, data.channel);
            ASSERT_EQ(resp.adc, data.adc);
        } catch (const std::runtime_error& e) {
            FAIL() << "Error: " << e.what();
        }
    }
    {
        SCOPED_TRACE("Check select adc 1 for channel 1");
        data_t data;
        data.channel = 1;
        data.adc = 1;
        try {
            auto resp = handler_t(tools.board).post(data, timeout);
            ASSERT_EQ(resp.channel, data.channel);
            ASSERT_EQ(resp.adc, data.adc);
        } catch (const std::runtime_error& e) {
            FAIL() << "Error: " << e.what();
        }
    }
}