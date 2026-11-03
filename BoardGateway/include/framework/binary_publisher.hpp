#pragma once

#include <ecal/pubsub/publisher.h>
#include <memory>
#include <string>
#include <gsl/gsl>

namespace SERVICE_NAMESPACE_PREFIX::framework
{
    template<typename T>
    class BinaryPublisher {
    public:
        using str = std::string;
        using data_t = gsl::span<const T>;
        using publisher_t = eCAL::CPublisher;

        BinaryPublisher(const str& topic)
            : _publisher{ std::make_shared<publisher_t>(topic) } {}

        void send(const data_t& data) {
            _publisher->Send(reinterpret_cast<const void*>(data.data()), data.size_bytes());
        }
        
    private:
        std::shared_ptr<publisher_t> _publisher{ nullptr };
    };
} // namespace SERVICE_NAMESPACE_PREFIX::framework
