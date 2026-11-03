#pragma once

#include <string>
#include <functional>
#include <ecal/ecal.h>
#include <ecal/msg/protobuf/subscriber.h>

namespace SERVICE_NAMESPACE_PREFIX::framework
{
    template<typename T>
    class ProtobufSubscriber {
    public:
        using str = std::string;
        using data_t = T;
        using data_handler_t = std::function<void(const data_t&)>;
        using subscriber_t = eCAL::protobuf::CSubscriber<data_t>;
        using callback_t = typename subscriber_t::MsgReceiveCallbackT;

        ProtobufSubscriber(const str& topic_name, data_handler_t data_handler) {
            _callback = [data_handler](const char*, const data_t& msg_, long long, long long, long long) {
                data_handler(msg_);
            };
            _subscriber = std::make_shared<subscriber_t>(topic_name);
        }

        void start() {
            _subscriber->AddReceiveCallback(_callback);
        }

        void stop() {
            _subscriber->RemReceiveCallback();
        }
    private:
        callback_t _callback;
        std::shared_ptr<subscriber_t> _subscriber{ nullptr };
    };
}
