#pragma once

#include "framework/binary_subscriber.hpp"
#include <string>
#include <functional>
#include <ecal/ecal.h>
#include <ecal/pubsub/subscriber.h>
#include <msgpack.hpp>

namespace SERVICE_NAMESPACE_PREFIX::framework
{
    template<typename T>
    class MsgpackSubscriber {
    public:
        using str = std::string;
        using data_t = T;
        using data_handler_t = std::function<void(const data_t&)>;
        using subscriber_t = BinarySubscriber<char>;

        MsgpackSubscriber(const str& topic_name, data_handler_t data_handler) {
            auto callback = [&, data_handler](gsl::span<const char> data) {
                data_handler(MsgpackSubscriber<T>::unpack(data));
            };
            _subscriber = std::make_shared<subscriber_t>(topic_name, callback);
        }

        void start() {
            _subscriber->start();
        }

        void stop() {
            _subscriber->stop();
        }
    private:
        std::shared_ptr<subscriber_t> _subscriber{ nullptr };

        static auto unpack(gsl::span<const char> s) -> T {
            data_t _;
            auto oh{ msgpack::unpack(s.data(), s.size()) };
            oh.get().convert(_);
            return _;
        }
    };
}
