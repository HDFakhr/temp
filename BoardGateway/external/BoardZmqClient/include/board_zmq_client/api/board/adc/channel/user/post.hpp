#pragma once

#include "zmq_rpc/json/json.hpp"
#include <string>
#include <cstdint>
#include <optional>

namespace nooyan::zmq_client::api::board::adc::channel::user
{
    struct Post {
        using str_view = std::string_view;
        using str = std::string;

        static constexpr auto path{ "/api/board/adc/channel/user" };
        static constexpr auto method{ "POST" };

        struct Channel {
            static constexpr auto none{ "" };
            static constexpr auto wb{ "wb" };
            static constexpr auto nb{ "nb" };
        };

        struct Data {
            struct Default {
                static constexpr auto channel{ Channel::none };
                static constexpr std::int64_t freq{ 0 };
                static constexpr std::int64_t sampling_rate{ 0 };
                static constexpr int adc{ -1 };
                static constexpr int channel_num{ -1 };
                static constexpr int adc_prec{ -1 };
            };

            struct Name {
                static constexpr auto channel{ "channel" };
                static constexpr auto freq{ "freq" };
                static constexpr auto sampling_rate{ "sampling_rate" };
                static constexpr auto adc{ "adc" };
                static constexpr auto channel_num{ "channel_num" };
                static constexpr auto adc_prec{ "prec" };
                static constexpr auto with_fractional_resampler{ "with_fractional_resampler" };
                static constexpr auto with_matched_filter{ "with_matched_filter" };
                static constexpr auto with_symbol_output{ "with_symbol_output" };
                static constexpr auto is_fixed{ "is_fixed" };
            };

            str channel{ Default::channel };
            std::int64_t freq{ Default::freq };
            std::int64_t sampling_rate{ Default::sampling_rate };
            int adc{ Default::adc };
            int channel_num{ Default::channel_num };
            int adc_prec{ Default::adc_prec };
            std::optional<bool> with_fractional_resampler{ false };
            std::optional<bool> with_matched_filter{ false };
            std::optional<bool> with_symbol_output{ false };
            std::optional<bool> is_fixed{ false };
        };

        struct Result {
            struct Default {
                static constexpr auto channel{ Channel::none };
                static constexpr std::int64_t adc_freq{ -1 };
                static constexpr std::int64_t channel_fs{ 0 };
                static constexpr std::int64_t freq{ -1 };
                static constexpr std::int64_t fs{ -1 };
                static constexpr std::int64_t nco{ -1 };
                static constexpr std::int64_t user_freq{ -1 };
                static constexpr int adc{ -1 };
                static constexpr int adc_prec{ -1 };
                static constexpr int channel_num{ -1 };
                static constexpr int cic_decimation{ -1 };
                static constexpr int cic_prec{ -1 };
                static constexpr int mgc_gain{ -1 };
                static constexpr int mode{ -1 };
                static constexpr int mxr_prec{ -1 };
                static constexpr int resampler_mu{ -1 };
                static constexpr int wb_chain{ -1 };
                static constexpr int wb_cic_prec{ -1 };
                static constexpr int wb_cic_rate{ -1 };
                static constexpr bool agc_enable{ false };
                static constexpr bool with_fractional_resampler{ false };
                static constexpr bool with_matched_filter{ false };
                static constexpr bool with_symbol_output{ false };
            };

            struct Name {
                static constexpr auto adc{ "adc" };
                static constexpr auto adc_freq{ "adc_freq" };
                static constexpr auto adc_prec{ "adc_prec" };
                static constexpr auto agc_enable{ "agc_enable" };
                static constexpr auto channel{ "channel" };
                static constexpr auto channel_fs{ "channel_fs" };
                static constexpr auto channel_num{ "channel_num" };
                static constexpr auto cic_decimation{ "cic_decimation" };
                static constexpr auto cic_prec{ "cic_prec" };
                static constexpr auto freq{ "freq" };
                static constexpr auto fs{ "fs" };
                static constexpr auto mgc_gain{ "mgc_gain" };
                static constexpr auto mode{ "mode" };
                static constexpr auto mxr_prec{ "mxr_prec" };
                static constexpr auto nco{ "nco" };
                static constexpr auto resampler_mu{ "resampler_mu" };
                static constexpr auto user_freq{ "user_freq" };
                static constexpr auto wb_chain{ "wb_chain" };
                static constexpr auto wb_cic_prec{ "wb_cic_prec" };
                static constexpr auto wb_cic_rate{ "wb_cic_rate" };
                static constexpr auto wb_enable{ "wb_enable" };
                static constexpr auto with_fractional_resampler{ "with_fractional_resampler" };
                static constexpr auto with_matched_filter{ "with_matched_filter" };
                static constexpr auto with_symbol_output{ "with_symbol_output" };
            };

            str channel{ Default::channel };
            std::int64_t adc_freq{ Default::adc_freq };
            std::int64_t channel_fs{ Default::channel_fs };
            std::int64_t freq{ Default::freq };
            std::int64_t fs{ Default::fs };
            std::int64_t nco{ Default::nco };
            std::int64_t user_freq{ Default::user_freq };
            int adc{ Default::adc };
            int adc_prec{ Default::adc_prec };
            int channel_num{ Default::channel_num };
            int cic_decimation{ Default::cic_decimation };
            int cic_prec{ Default::cic_prec };
            int mgc_gain{ Default::mgc_gain };
            int mode{ Default::mode };
            int mxr_prec{ Default::mxr_prec };
            int resampler_mu{ Default::resampler_mu };
            int wb_chain{ Default::wb_chain };
            int wb_cic_prec{ Default::wb_cic_prec };
            int wb_cic_rate{ Default::wb_cic_rate };
            bool agc_enable{ Default::agc_enable };
            bool with_fractional_resampler{ Default::with_fractional_resampler };
            bool with_matched_filter{ Default::with_matched_filter };
            bool with_symbol_output{ Default::with_symbol_output };
        };
    };

    static void to_json(nlohmann::json& j, const Post::Data& data) {
        using data_t = Post::Data;
        zmq_rpc::json::add_to_json(j, data_t::Name::channel, data.channel, std::string(data_t::Default::channel));
        zmq_rpc::json::add_to_json(j, data_t::Name::freq, data.freq, data_t::Default::freq);
        zmq_rpc::json::add_to_json(j, data_t::Name::sampling_rate, data.sampling_rate, data_t::Default::sampling_rate);
        zmq_rpc::json::add_to_json(j, data_t::Name::adc, data.adc, data_t::Default::adc);
        zmq_rpc::json::add_to_json(j, data_t::Name::channel_num, data.channel_num, data_t::Default::channel_num);
        zmq_rpc::json::add_to_json(j, data_t::Name::adc_prec, data.adc_prec, data_t::Default::adc_prec);
        zmq_rpc::json::add_to_json(j, data_t::Name::with_fractional_resampler, data.with_fractional_resampler);
        zmq_rpc::json::add_to_json(j, data_t::Name::with_matched_filter, data.with_matched_filter);
        zmq_rpc::json::add_to_json(j, data_t::Name::with_symbol_output, data.with_symbol_output);
        zmq_rpc::json::add_to_json(j, data_t::Name::is_fixed, data.is_fixed);
    }

    static void from_json(const nlohmann::json& j, Post::Result& result) {
        using result_t = Post::Result;

        zmq_rpc::json::get_from_json_to(j, result_t::Name::channel, result.channel);
        zmq_rpc::json::get_from_json_to(j, result_t::Name::adc_freq, result.adc_freq);
        zmq_rpc::json::get_from_json_to(j, result_t::Name::channel_fs, result.channel_fs);
        zmq_rpc::json::get_from_json_to(j, result_t::Name::freq, result.freq);
        zmq_rpc::json::get_from_json_to(j, result_t::Name::fs, result.fs);
        zmq_rpc::json::get_from_json_to(j, result_t::Name::nco, result.nco);
        zmq_rpc::json::get_from_json_to(j, result_t::Name::user_freq, result.user_freq);
        zmq_rpc::json::get_from_json_to(j, result_t::Name::adc, result.adc);
        zmq_rpc::json::get_from_json_to(j, result_t::Name::adc_prec, result.adc_prec);
        zmq_rpc::json::get_from_json_to(j, result_t::Name::channel_num, result.channel_num);
        zmq_rpc::json::get_from_json_to(j, result_t::Name::cic_decimation, result.cic_decimation);
        zmq_rpc::json::get_from_json_to(j, result_t::Name::cic_prec, result.cic_prec);
        zmq_rpc::json::get_from_json_to(j, result_t::Name::mgc_gain, result.mgc_gain);
        zmq_rpc::json::get_from_json_to(j, result_t::Name::mode, result.mode);
        zmq_rpc::json::get_from_json_to(j, result_t::Name::mxr_prec, result.mxr_prec);
        zmq_rpc::json::get_from_json_to(j, result_t::Name::resampler_mu, result.resampler_mu);
        zmq_rpc::json::get_from_json_to(j, result_t::Name::wb_chain, result.wb_chain);
        zmq_rpc::json::get_from_json_to(j, result_t::Name::wb_cic_prec, result.wb_cic_prec);
        zmq_rpc::json::get_from_json_to(j, result_t::Name::wb_cic_rate, result.wb_cic_rate);
        zmq_rpc::json::get_from_json_to(j, result_t::Name::agc_enable, result.agc_enable);
        zmq_rpc::json::get_from_json_to(j, result_t::Name::with_fractional_resampler, result.with_fractional_resampler);
        zmq_rpc::json::get_from_json_to(j, result_t::Name::with_matched_filter, result.with_matched_filter);
        zmq_rpc::json::get_from_json_to(j, result_t::Name::with_symbol_output, result.with_symbol_output);
    }
}