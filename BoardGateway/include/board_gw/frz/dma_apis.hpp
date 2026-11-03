#pragma once

#include <zmq.hpp>
#include "board_zmq_client/tools.hpp"
#include "board_gw/frz/conn_config.hpp"
#include "board_zmq_client/utils/dma_channel_num.hpp"
#include "board_zmq_client/api/dma/data/get.hpp"
#include <memory>

namespace pooyan::board::frz
{
    class DmaApis {
    public:
        using zmq_context_t = zmq::context_t;
        using tools_t = nooyan::zmq_client::Tools;
        using conn_config_t = ConnConfig;
        using channel_t = nooyan::zmq_client::utils::Dma::ChannelType;

        template<channel_t T, int N, int ADC>
        static constexpr auto channel_num_v = nooyan::zmq_client::utils::Dma::ChannelNum<T, N, ADC>::value;

        struct Data {
            using response_t = nooyan::zmq_client::api::dma::data::Get::Result;
        };

        DmaApis(std::shared_ptr<zmq_context_t> zmq_ctx)
            : _tools{ zmq_ctx } {}

        template<channel_t T, int N, int ADC>
        void initial(const conn_config_t& config) {
            auto ch = channel_num_v<T, N, ADC>;
            if (ch == -1)
                throw std::runtime_error("dma for this channel is not defined");
            _channel = static_cast<std::size_t>(ch);
            _initial(config);
        }

        Data::response_t data(std::int64_t size, std::int64_t offset, int timeout_ms = -1);

    private:
        tools_t _tools;
        std::size_t _channel{ 0 };

        void _initial(const conn_config_t& config);
    };
}