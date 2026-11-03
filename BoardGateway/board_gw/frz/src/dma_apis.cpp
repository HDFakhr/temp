#include "board_gw/frz/dma_apis.hpp"
#include "board_zmq_client/pipeline.hpp"
#include "board_zmq_client/api/dma/start/handler.hpp"
#include "board_zmq_client/api/dma/status/handler.hpp"
#include "board_zmq_client/api/dma/data/handler.hpp"
#include <fmt/format.h>
#include <thread>
#include <chrono>

using pooyan::board::frz::DmaApis;

namespace
{
    static constexpr std::int64_t max_chunk_size{ 1024 * 1024 };

    struct DmaHelper {
        using tools_t = DmaApis::tools_t;

        tools_t& tools;
        std::size_t channel{ 0 };

        DmaHelper(tools_t& tools_, std::size_t channel_) : tools{ tools_ }, channel{ channel_ } {}

        nooyan::zmq_client::api::dma::start::Post::Result start(std::int64_t size, int timeout_ms = -1) {
            nooyan::zmq_client::api::dma::start::Post::Data req;
            req.size = size;
            return nooyan::zmq_client::api::dma::start::Handler(tools.dma[channel])
                .post(req, timeout_ms);
        }

        nooyan::zmq_client::api::dma::status::Get::Result status(int timeout_ms = -1) {
            return nooyan::zmq_client::api::dma::status::Handler(tools.dma[channel])
                .get(timeout_ms);
        }

        nooyan::zmq_client::api::dma::data::Get::Result data(std::int64_t chunk_size, std::int64_t offset, int timeout_ms = -1) {
            nooyan::zmq_client::api::dma::data::Get::Data req;
            req.offset = offset;
            req.chunk_size = chunk_size;
            return nooyan::zmq_client::api::dma::data::Handler(tools.dma[channel])
                .data(req, timeout_ms);
        }
    };

    struct DownloadHelper {
        using tools_t = DmaApis::tools_t;

        tools_t& tools;
        std::size_t channel{ 0 };

        DownloadHelper(tools_t& tools_, std::size_t channel_) : tools{ tools_ }, channel{ channel_ } {}

        DmaApis::Data::response_t single_chunk(std::int64_t size, std::int64_t offset, int timeout_ms) {
            auto new_size = size + offset;
            auto start_resp = DmaHelper(tools, channel).start(new_size, timeout_ms);
            while (true) {
                auto status_resp = DmaHelper(tools, channel).status(timeout_ms);
                if (status_resp.size >= new_size)
                    break;
                std::this_thread::sleep_for(std::chrono::milliseconds(10));
            }

            return DmaHelper(tools, channel).data(size, offset, timeout_ms);
        }

        DmaApis::Data::response_t multiple_chunk(std::int64_t size, std::int64_t offset, int timeout_ms) {
            using data_t = DmaApis::Data::response_t::value_type;
            DmaApis::Data::response_t output;
            auto buffer_size = size / sizeof(data_t);
            output.reserve(buffer_size);

            auto new_size = size + offset;
            auto start_resp = DmaHelper(tools, channel).start(new_size, timeout_ms);

            

            while (true) {
                auto status_resp = DmaHelper(tools, channel).status(timeout_ms);
                if (status_resp.size >= new_size)
                    break;
            }
            
            std::int64_t recv_size{ 0 };
            /*auto buf = DmaHelper(tools, channel).data(size, offset, timeout_ms);*/
			while (recv_size < size) {
				auto remaining = size - recv_size;
				auto chunk_size = std::min(remaining, max_chunk_size);

				if (chunk_size == 0)
					break;

				auto buf = DmaHelper(tools, channel).data(chunk_size, offset, timeout_ms);
				output.insert(output.end(), buf.begin(), buf.end());
				recv_size += buf.size() * sizeof(data_t);
				offset += buf.size() * sizeof(data_t);
			}
            return output;
        }
    };
}

void DmaApis::_initial(const conn_config_t& config) {
    nooyan::zmq_client::Pipeline(_tools)
        .initial_dma_client(config.ip, config.port, _channel);
}

auto DmaApis::data(std::int64_t size, std::int64_t offset, int timeout_ms) -> Data::response_t {
    if (!_tools.dma[_channel].socket)
        throw std::runtime_error(fmt::format("dma is not initialzed for channel={}", _channel));
    if (size > max_chunk_size)
        return DownloadHelper(_tools, _channel).multiple_chunk(size, offset, timeout_ms);
    return DownloadHelper(_tools, _channel).single_chunk(size, offset, timeout_ms);
}