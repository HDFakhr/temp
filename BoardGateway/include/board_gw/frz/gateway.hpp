#pragma once

#include <zmq.hpp>
#include "board_gw/frz/config.hpp"
#include "board_gw/frz/board_apis.hpp"
#include "board_gw/frz/capture_apis.hpp"
#include "board_gw/frz/spectrum_apis.hpp"
#include "board_gw/frz/dma_apis.hpp"
#include <memory>
#include <array>
#include <filesystem>

namespace pooyan::board::frz
{
    class Gateway {
    public:
        using zmq_context_t = zmq::context_t;
        using config_t = Config;
        using board_apis_t = BoardApis;
        using capture_apis_t = CaptureApis;
        using spectrum_apis_t = SpectrumApis;
        using dma_apis_t = DmaApis;

        using ConfigDDC = board_apis_t::ConfigDDCByUser;
        using Spectrum = spectrum_apis_t::Spectrum;
        using DmaData = dma_apis_t::Data;
        
        static constexpr auto SPECTRUM_CHANNELS{ nooyan::zmq_client::Tools::SPECTRUM_CHANNELS };
        static constexpr auto DMA_CHANNELS{ nooyan::zmq_client::Tools::DMA_CHANNELS };

        Gateway();

        void initial(const config_t& config);
        void initial(const std::filesystem::path& config_path);
        void save_ip(const std::filesystem::path& config_path, std::string board_ip);

        ConfigDDC::response_t config_ddc(const ConfigDDC::data_t& data, int timeout_ms = -1);
        board_apis_t::SetAdcOfChannel::response_t set_adc_of_channel_0(int adc, int timeout_ms = -1);
        board_apis_t::SetAdcOfChannel::response_t set_adc_of_channel_1(int adc, int timeout_ms = -1);
        board_apis_t::ChannelAttenuation::response_t set_channel_attenuation(int channel, int attenuation, int timeout_ms = -1);
        capture_apis_t::Start::response_t start_capture(const capture_apis_t::Start::data_t& data, int timeout_ms = -1);
        capture_apis_t::Stop::response_t stop_capture(int timeout_ms = -1);
        capture_apis_t::Status::response_t capture_status(int timeout_ms = -1);
        capture_apis_t::Device::response_t capture_device_info(int timeout_ms = -1);
        Spectrum::response_t spectrum(int channel = 0, int timeout_ms = -1);
        DmaData::response_t dma_wb_0_0(std::int64_t size, std::int64_t offset, int timeout_ms = -1);
        DmaData::response_t dma_nb_0_0(std::int64_t size, std::int64_t offset, int timeout_ms = -1);
        DmaData::response_t dma_nb_0_1(std::int64_t size, std::int64_t offset, int timeout_ms = -1);
        DmaData::response_t dma_nb_0_2(std::int64_t size, std::int64_t offset, int timeout_ms = -1);
        DmaData::response_t dma_nb_0_3(std::int64_t size, std::int64_t offset, int timeout_ms = -1);

        DmaData::response_t dma_wb_1_0(std::int64_t size, std::int64_t offset, int timeout_ms = -1);
        DmaData::response_t dma_nb_1_0(std::int64_t size, std::int64_t offset, int timeout_ms = -1);
        DmaData::response_t dma_nb_1_1(std::int64_t size, std::int64_t offset, int timeout_ms = -1);
        DmaData::response_t dma_nb_1_2(std::int64_t size, std::int64_t offset, int timeout_ms = -1);
        DmaData::response_t dma_nb_1_3(std::int64_t size, std::int64_t offset, int timeout_ms = -1);
    private:
        std::shared_ptr<zmq_context_t> _zmq_ctx{ nullptr };
        std::shared_ptr<board_apis_t> _board_apis{ nullptr };
        std::shared_ptr<capture_apis_t> _capture_apis{ nullptr };
        std::array<std::shared_ptr<spectrum_apis_t>, SPECTRUM_CHANNELS> _spectrum_apis{ nullptr };
        std::array<std::shared_ptr<dma_apis_t>, DMA_CHANNELS> _dma_apis{ nullptr };

        void _construct_apis();
    };
}