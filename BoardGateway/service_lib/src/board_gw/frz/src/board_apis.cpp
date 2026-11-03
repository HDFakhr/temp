#include "board_gw/frz/board_apis.hpp"
#include "board_zmq_client/pipeline.hpp"
#include "board_zmq_client/api/board/adc/channel/user/handler.hpp"
#include "board_zmq_client/api/board/channel/adc/handler.hpp"
#include "board_zmq_client/api/board/attenuation/handler.hpp"

using pooyan::board::frz::BoardApis;

void BoardApis::initial(const conn_config_t& config) {
    nooyan::zmq_client::Pipeline(_tools)
        .initial_board_client(config.ip, config.port);
}

auto BoardApis::config_ddc_by_user(const ConfigDDCByUser::data_t& data, int timeout) -> ConfigDDCByUser::response_t {
    if (!_tools.board.socket)
        throw std::runtime_error("board apis is not initialized");
    return nooyan::zmq_client::api::board::adc::channel::user::Handler(_tools.board)
        .post(data, timeout);
}

auto BoardApis::set_adc_of_channel(const SetAdcOfChannel::data_t& data, int timeout) -> SetAdcOfChannel::response_t {
    if (!_tools.board.socket)
        throw std::runtime_error("board apis is not initialized");
    return nooyan::zmq_client::api::board::channel::adc::Handler(_tools.board)
        .post(data, timeout);
}

auto BoardApis::set_attenuation(const ChannelAttenuation::data_t& data, int timeout) -> ChannelAttenuation::response_t {
    if (!_tools.board.socket)
        throw std::runtime_error("board apis is not initialized");
    return nooyan::zmq_client::api::board::attenuation::Handler(_tools.board)
        .post(data, timeout);
}