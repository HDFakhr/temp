#pragma once

#include "Ipublisher.hpp"
#include <string>

namespace SERVICE_NAMESPACE_PREFIX::framework
{
    template<typename PublisherT>
    class PublisherBuilder {
    public:
        using str = std::string;
        using publisher_t = PublisherT;
        using data_t = typename publisher_t::data_t;
        using ipublisher_t = IPublisher<data_t>;

        static ipublisher_t build(const str& topic) {
            auto publisher{ std::make_shared<publisher_t>(topic) };
            ipublisher_t _;
            _.send = [publisher](const data_t& data) {
                publisher->send(data);
            };
            return _;
        }
    };
} // namespace SERVICE_NAMESPACE_PREFIX::framework
