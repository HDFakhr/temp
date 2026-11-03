#include "board_gw/frz/gateway.hpp"
#include "board_gw/frz/config_handler.hpp"
#include "board_zmq_client/utils/dma_channel_num.hpp"

using pooyan::board::frz::Gateway;
using nooyan::zmq_client::utils::Dma;

namespace
{
    static constexpr std::array<bool, Gateway::SPECTRUM_CHANNELS> spectrum_mask{ false, true, false, true, true, true, true, true };

    template<Dma::ChannelType T, int N, int ADC>
    struct DMA {
        static constexpr auto mask{ true };
    };

    template<>
    struct DMA<Dma::ChannelType::NB, 0, 0> {
        static constexpr auto mask{ false };
    };

    template<>
    struct DMA<Dma::ChannelType::WB, 0, 0> {
        static constexpr auto mask{ false };
    };

    template<>
    struct DMA<Dma::ChannelType::NB, 0, 1> {
        static constexpr auto mask{ false };
    };

    template<>
    struct DMA<Dma::ChannelType::WB, 0, 1> {
        static constexpr auto mask{ false };
    };
    
    template<>
    struct DMA<Dma::ChannelType::NB, 1, 0> {
        static constexpr auto mask{ false };
    };

    template<>
    struct DMA<Dma::ChannelType::NB, 1, 1> {
        static constexpr auto mask{ false };
    };

    template<>
    struct DMA<Dma::ChannelType::NB, 2, 0> {
        static constexpr auto mask{ false };
    };

    template<>
    struct DMA<Dma::ChannelType::NB, 2, 1> {
        static constexpr auto mask{ false };
    };

    template<>
    struct DMA<Dma::ChannelType::NB, 3, 0> {
        static constexpr auto mask{ false };
    };

    template<>
    struct DMA<Dma::ChannelType::NB, 3, 1> {
        static constexpr auto mask{ false };
    };

    template<Dma::ChannelType T, int N, int ADC>
    static void initial_dma_api(
        std::shared_ptr<Gateway::dma_apis_t> dma_api, const Gateway::config_t& config
    ) {
        static constexpr auto channel{ Dma::ChannelNum<T, N, ADC>::value };
        if constexpr (DMA<T, N, ADC>::mask) {
            return;
        } else {
            dma_api->initial<T, N, ADC>(pooyan::board::frz::make_conn_config(config.board_ip, config.ports.dma[channel]));
        }
    }
}

Gateway::Gateway() {
    _zmq_ctx = std::make_shared<zmq_context_t>();
    _construct_apis();
}

void Gateway::_construct_apis() {
    _board_apis = std::make_shared<board_apis_t>(_zmq_ctx);
    _capture_apis = std::make_shared<capture_apis_t>(_zmq_ctx);
    std::generate(
        std::begin(_spectrum_apis), std::end(_spectrum_apis),
        [&]() {
            return std::make_shared<Gateway::spectrum_apis_t>(_zmq_ctx);
        }
    );
    std::generate(
        std::begin(_dma_apis), std::end(_dma_apis),
        [&]() {
            return std::make_shared<Gateway::dma_apis_t>(_zmq_ctx);
        }
    );
}

void Gateway::initial(const config_t& config) {
    _construct_apis();
    _board_apis->initial(make_conn_config(config.board_ip, config.ports.board));
    _capture_apis->initial(make_conn_config(config.board_ip, config.ports.capture));
    for (std::size_t index = 0; index < SPECTRUM_CHANNELS; ++index) {
        if (spectrum_mask[index])
            continue;;
        _spectrum_apis[index]->initial(pooyan::board::frz::make_conn_config(config.board_ip, config.ports.spectrums[index]), index);
    }

    initial_dma_api<Dma::ChannelType::WB, 0, 0>(_dma_apis[Dma::ChannelNum<Dma::ChannelType::WB, 0, 0>::value], config);
    initial_dma_api<Dma::ChannelType::NB, 0, 0>(_dma_apis[Dma::ChannelNum<Dma::ChannelType::NB, 0, 0>::value], config);
    initial_dma_api<Dma::ChannelType::NB, 1, 0>(_dma_apis[Dma::ChannelNum<Dma::ChannelType::NB, 1, 0>::value], config);
    initial_dma_api<Dma::ChannelType::NB, 2, 0>(_dma_apis[Dma::ChannelNum<Dma::ChannelType::NB, 2, 0>::value], config);
    initial_dma_api<Dma::ChannelType::NB, 3, 0>(_dma_apis[Dma::ChannelNum<Dma::ChannelType::NB, 3, 0>::value], config);

    initial_dma_api<Dma::ChannelType::WB, 0, 1>(_dma_apis[Dma::ChannelNum<Dma::ChannelType::WB, 0, 1>::value], config);
    initial_dma_api<Dma::ChannelType::NB, 0, 1>(_dma_apis[Dma::ChannelNum<Dma::ChannelType::NB, 0, 1>::value], config);
    initial_dma_api<Dma::ChannelType::NB, 1, 1>(_dma_apis[Dma::ChannelNum<Dma::ChannelType::NB, 1, 1>::value], config);
    initial_dma_api<Dma::ChannelType::NB, 2, 1>(_dma_apis[Dma::ChannelNum<Dma::ChannelType::NB, 2, 1>::value], config);
    initial_dma_api<Dma::ChannelType::NB, 3, 1>(_dma_apis[Dma::ChannelNum<Dma::ChannelType::NB, 3, 1>::value], config);
}

void Gateway::initial(const std::filesystem::path& config_path) {
    auto config = ConfigHandler::load_from_json(config_path);
    initial(config);
}

void Gateway::save_ip(const std::filesystem::path& config_path, std::string board_ip) {
	auto config = ConfigHandler::load_from_json(config_path);
    if (config.board_ip != board_ip)
    {
        config.board_ip = board_ip;        
        ConfigHandler::save_to_json(config, config_path);
    }
}

auto Gateway::config_ddc(const ConfigDDC::data_t& data, int timeout_ms) -> ConfigDDC::response_t {
    return _board_apis->config_ddc_by_user(data, timeout_ms);
}

auto Gateway::set_adc_of_channel_0(int adc, int timeout_ms) -> board_apis_t::SetAdcOfChannel::response_t {
    board_apis_t::SetAdcOfChannel::data_t data;
    data.channel = 0;
    data.adc = adc % 2;
    return _board_apis->set_adc_of_channel(data, timeout_ms);
}

auto Gateway::set_adc_of_channel_1(int adc, int timeout_ms) -> board_apis_t::SetAdcOfChannel::response_t {
    board_apis_t::SetAdcOfChannel::data_t data;
    data.channel = 1;
    data.adc = adc % 2;
    return _board_apis->set_adc_of_channel(data, timeout_ms);
}

auto Gateway::set_channel_attenuation(int channel, int attenuation, int timeout_ms) -> board_apis_t::ChannelAttenuation::response_t {
    board_apis_t::ChannelAttenuation::data_t data;
    data.channel = channel;
    data.attenuation = attenuation;
    return _board_apis->set_attenuation(data, timeout_ms);
}


auto Gateway::start_capture(const capture_apis_t::Start::data_t& data, int timeout_ms) -> capture_apis_t::Start::response_t {
    return _capture_apis->start(data, timeout_ms);
}

auto Gateway::stop_capture(int timeout_ms) -> capture_apis_t::Stop::response_t {
    return _capture_apis->stop(timeout_ms);
}

auto Gateway::capture_status(int timeout_ms) -> capture_apis_t::Status::response_t {
    return _capture_apis->status(timeout_ms);
}

auto Gateway::capture_device_info(int timeout_ms) -> capture_apis_t::Device::response_t {
    return _capture_apis->device_info(timeout_ms);
}

auto Gateway::spectrum(int channel, int timeout_ms) -> Spectrum::response_t {
    return _spectrum_apis[channel]->spectrum(timeout_ms);
}

auto Gateway::dma_wb_0_0(std::int64_t size, std::int64_t offset, int timeout_ms) -> DmaData::response_t {
    return _dma_apis[Dma::ChannelNum<Dma::ChannelType::WB, 0, 0>::value]->data(size, offset, timeout_ms);
}

auto Gateway::dma_nb_0_0(std::int64_t size, std::int64_t offset, int timeout_ms) -> DmaData::response_t {
    return _dma_apis[Dma::ChannelNum<Dma::ChannelType::NB, 0, 0>::value]->data(size, offset, timeout_ms);
}

auto Gateway::dma_nb_0_1(std::int64_t size, std::int64_t offset, int timeout_ms) -> DmaData::response_t {
    return _dma_apis[Dma::ChannelNum<Dma::ChannelType::NB, 1, 0>::value]->data(size, offset, timeout_ms);
}

auto Gateway::dma_nb_0_2(std::int64_t size, std::int64_t offset, int timeout_ms) -> DmaData::response_t {
    return _dma_apis[Dma::ChannelNum<Dma::ChannelType::NB, 2, 0>::value]->data(size, offset, timeout_ms);
}

auto Gateway::dma_nb_0_3(std::int64_t size, std::int64_t offset, int timeout_ms) -> DmaData::response_t {
    return _dma_apis[Dma::ChannelNum<Dma::ChannelType::NB, 3, 0>::value]->data(size, offset, timeout_ms);
}

auto Gateway::dma_wb_1_0(std::int64_t size, std::int64_t offset, int timeout_ms) -> DmaData::response_t {
    return _dma_apis[Dma::ChannelNum<Dma::ChannelType::WB, 0, 1>::value]->data(size, offset, timeout_ms);
}

auto Gateway::dma_nb_1_0(std::int64_t size, std::int64_t offset, int timeout_ms) -> DmaData::response_t {
    return _dma_apis[Dma::ChannelNum<Dma::ChannelType::NB, 0, 1>::value]->data(size, offset, timeout_ms);
}

auto Gateway::dma_nb_1_1(std::int64_t size, std::int64_t offset, int timeout_ms) -> DmaData::response_t {
    return _dma_apis[Dma::ChannelNum<Dma::ChannelType::NB, 1, 1>::value]->data(size, offset, timeout_ms);
}

auto Gateway::dma_nb_1_2(std::int64_t size, std::int64_t offset, int timeout_ms) -> DmaData::response_t {
    return _dma_apis[Dma::ChannelNum<Dma::ChannelType::NB, 2, 1>::value]->data(size, offset, timeout_ms);
}

auto Gateway::dma_nb_1_3(std::int64_t size, std::int64_t offset, int timeout_ms) -> DmaData::response_t {
    return _dma_apis[Dma::ChannelNum<Dma::ChannelType::NB, 3, 1>::value]->data(size, offset, timeout_ms);
}