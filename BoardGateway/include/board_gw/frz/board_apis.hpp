#pragma once

#include <zmq.hpp>
#include "board_zmq_client/tools.hpp"
#include "board_gw/frz/conn_config.hpp"
#include "board_zmq_client/api/board/adc/channel/user/post.hpp"
#include "board_zmq_client/api/board/channel/adc/post.hpp"
#include "board_zmq_client/api/board/attenuation/post.hpp"
#include <memory>

namespace pooyan::board::frz
{
    class BoardApis {
    public:
        using zmq_context_t = zmq::context_t;
        using tools_t = nooyan::zmq_client::Tools;
        using conn_config_t = ConnConfig;

        struct ConfigDDCByUser {
            using channel_t = nooyan::zmq_client::api::board::adc::channel::user::Post::Channel;
            using data_t = nooyan::zmq_client::api::board::adc::channel::user::Post::Data;
            using response_t = nooyan::zmq_client::api::board::adc::channel::user::Post::Result;
        };

        struct SetAdcOfChannel {
            using data_t = nooyan::zmq_client::api::board::channel::adc::Post::Data;
            using response_t = nooyan::zmq_client::api::board::channel::adc::Post::Result;
        };

        struct ChannelAttenuation {
            using data_t = nooyan::zmq_client::api::board::attenuation::Post::Data;
            using response_t = nooyan::zmq_client::api::board::attenuation::Post::Result;
        };

        BoardApis(std::shared_ptr<zmq_context_t> zmq_ctx)
            : _tools{ zmq_ctx } {}

        void initial(const conn_config_t& config);
        ConfigDDCByUser::response_t config_ddc_by_user(const ConfigDDCByUser::data_t& data, int timeout = -1);
        SetAdcOfChannel::response_t set_adc_of_channel(const SetAdcOfChannel::data_t& data, int timeout = -1);
        ChannelAttenuation::response_t set_attenuation(const ChannelAttenuation::data_t& data, int timeout = -1);
    private:
        tools_t _tools;
    };
}