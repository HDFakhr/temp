#pragma once

#include "zmq_rpc/json/json.hpp"
#include <string>
#include <cstdint>
#include <optional>
#include <vector>

namespace nooyan::zmq_client::api::dma::data
{
    struct Get {
        using str_view = std::string_view;
        using str = std::string;

        static constexpr auto path{ "/api/dma/data" };
        static constexpr auto method{ "GET" };

        struct Data {
            struct Default {
                static constexpr std::int64_t chunk_size{ -1 };
                static constexpr std::int64_t offset{ -1 };
            };

            struct Name {
                static constexpr auto chunk_size{ "chunk_size" };
                static constexpr auto offset{ "offset" };
            };

            std::int64_t chunk_size{ Default::chunk_size };
            std::int64_t offset{ Default::offset };
        };

        using Result = std::vector<short>;
    };

    static void to_json(nlohmann::json& j, const Get::Data& data) {
        using data_t = Get::Data;
        zmq_rpc::json::add_to_json(j, data_t::Name::chunk_size, data.chunk_size, data_t::Default::chunk_size);
        zmq_rpc::json::add_to_json(j, data_t::Name::offset, data.offset, data_t::Default::offset);
    }
}