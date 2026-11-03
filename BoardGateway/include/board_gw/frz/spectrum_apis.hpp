#pragma once

#include <zmq.hpp>
#include "board_zmq_client/tools.hpp"
#include "board_gw/frz/conn_config.hpp"
#include "board_zmq_client/api/spectrum/get.hpp"
#include <memory>

namespace pooyan::board::frz
{
    class SpectrumApis {
    public:
        using zmq_context_t = zmq::context_t;
        using tools_t = nooyan::zmq_client::Tools;
        using conn_config_t = ConnConfig;
        
        struct Spectrum {
            using response_t = nooyan::zmq_client::api::spectrum::Get::Result;
        };


        SpectrumApis(std::shared_ptr<zmq_context_t> zmq_ctx)
            : _tools{ zmq_ctx } {}

        void initial(const conn_config_t& config, std::size_t channel);
        Spectrum::response_t spectrum(int timeout_ms = -1);
    private:
        tools_t _tools;
        std::size_t channel{ 0 };
    };
}