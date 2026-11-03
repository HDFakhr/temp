#pragma once

#include "Isubscriber.hpp"
#include <functional>

namespace SERVICE_NAMESPACE_PREFIX::framework
{
    template<typename SubscriberT>
    class SubscriberBuilder {
    public:
        using isubscriber_t = ISubscriber;
        using subscriber_t = SubscriberT;
        using data_handler_t = typename subscriber_t::data_handler_t;
        using str = typename subscriber_t::str;

        static isubscriber_t build(const str& topic_name, data_handler_t data_handler) {
            auto subscriber = std::make_shared<subscriber_t>(
                topic_name, data_handler
            );
            isubscriber_t _;
            _.start = [subscriber]() {
                subscriber->start();
            };
            _.stop = [subscriber]() {
                subscriber->stop();
            };
            return _;
        }
    };
}