#pragma once

#include <ecal/pubsub/publisher.h>
#include <memory>
#include <string>
#include <msgpack.hpp>
#include <sstream>

namespace SERVICE_NAMESPACE_PREFIX::framework
{
    template<typename T>
    class MsgpackPublisher {
    public:
        using str = std::string;
        using data_t = T;
        using publisher_t = eCAL::CPublisher;

        MsgpackPublisher(const str& topic)
            : _publisher{ std::make_shared<publisher_t>(topic) } {}

        void send(const data_t& data) {
            auto msg{ MsgpackPublisher<T>::pack(data) };
            _publisher->Send(msg.data(), msg.size());
        }
        
    private:
        std::shared_ptr<publisher_t> _publisher{ nullptr };

        static std::string pack(const data_t& d) {
            std::stringstream ss;
            msgpack::pack(ss, d);
            return ss.str();
        }
    };
} // namespace SERVICE_NAMESPACE_PREFIX::framework
