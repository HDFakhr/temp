#include "board_gw/frz/spectrum_apis.hpp"
#include "board_zmq_client/pipeline.hpp"
#include "board_zmq_client/api/spectrum/handler.hpp"
#include <fmt/format.h>

using pooyan::board::frz::SpectrumApis;

void SpectrumApis::initial(const conn_config_t& config, std::size_t channel_) {
    nooyan::zmq_client::Pipeline(_tools)
        .initial_spectrum_client(config.ip, config.port, channel_);
    channel = channel_;
}

auto SpectrumApis::spectrum(int timeout_ms) -> Spectrum::response_t {
    if (!_tools.spectrums[channel].socket)
        throw std::runtime_error(fmt::format("spectrum is not initialzed for channel={}", channel));
    return nooyan::zmq_client::api::spectrum::Handler(_tools.spectrums[channel])
        .get(timeout_ms);
}