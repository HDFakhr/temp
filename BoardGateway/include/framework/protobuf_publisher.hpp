#pragma once

#include <ecal/msg/protobuf/publisher.h>
#include <memory>
#include <string>

namespace SERVICE_NAMESPACE_PREFIX::framework
{
    template<typename T>
    class ProtobufPublisher {
    public:
        using str = std::string;
        using data_t = T;
        using publisher_t = eCAL::protobuf::CPublisher<data_t>;

        ProtobufPublisher(const str& topic)
            : _publisher{ std::make_shared<publisher_t>(topic) } {}

        void send(const data_t& data) {
            _publisher->Send(data);
        }
        
    private:
        std::shared_ptr<publisher_t> _publisher{ nullptr };
    };
} // namespace SERVICE_NAMESPACE_PREFIX::framework
