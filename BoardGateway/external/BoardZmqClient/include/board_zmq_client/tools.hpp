#pragma once

#include "zmq_rpc/client_tools.hpp"
#include <cstdlib>
#include <array>
#include <zmq.hpp>
#include <memory>

namespace nooyan::zmq_client
{
    struct Tools {
        using size_t = std::size_t;
        using zmq_context_t = zmq::context_t;
        using zmq_client_tools_t = zmq_rpc::ClientTools;
        static constexpr size_t SPECTRUM_CHANNELS{ 8 };
        static constexpr size_t DMA_CHANNELS{ 10 };

        std::shared_ptr<zmq_context_t> zmq_context{ nullptr };
        zmq_client_tools_t board;
        zmq_client_tools_t capture;
        std::array<zmq_client_tools_t, SPECTRUM_CHANNELS> spectrums;
        std::array<zmq_client_tools_t, DMA_CHANNELS> dma;

        Tools() : zmq_context{ std::make_shared<zmq_context_t>() } {}
        Tools(int io_threads_) : zmq_context{ std::make_shared<zmq_context_t>(io_threads_) } {}
        Tools(std::shared_ptr<zmq_context_t> ctx) : zmq_context{ ctx } {}
    };
}