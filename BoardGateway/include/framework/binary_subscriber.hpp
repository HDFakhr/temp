#pragma once

#include <string>
#include <functional>
#include <ecal/ecal.h>
#include <ecal/pubsub/subscriber.h>
#include <gsl/gsl>

namespace SERVICE_NAMESPACE_PREFIX::framework
{
    template<typename T>
    class BinarySubscriber {
    public:
        using str = std::string;
        using data_t = T;
        using data_handler_t = std::function<void(gsl::span<const data_t>)>;
        using subscriber_t = eCAL::CSubscriber;
        using callback_t = eCAL::ReceiveCallbackT;

        BinarySubscriber(const str& topic_name, data_handler_t data_handler) {
            _callback = [data_handler](const eCAL::STopicId&, const eCAL::SDataTypeInformation&, const eCAL::SReceiveCallbackData& data_) {
                data_handler(gsl::make_span(reinterpret_cast<const data_t*>(data_.buffer), data_.buffer_size / sizeof(data_t)));
            };
            _subscriber = std::make_shared<subscriber_t>(topic_name);
        }

        void start() {
            _subscriber->SetReceiveCallback(_callback);
        }

        void stop() {
            _subscriber->RemoveReceiveCallback();
        }
    private:
        callback_t _callback;
        std::shared_ptr<subscriber_t> _subscriber{ nullptr };
    };
}
