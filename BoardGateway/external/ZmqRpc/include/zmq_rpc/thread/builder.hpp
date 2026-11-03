#pragma once

#include <memory>
#include "zmq_rpc/thread/Ithread.hpp"
#include "zmq_rpc/thread/thread.hpp"

namespace zmq_rpc::thread
{
    class ThreadBuilder {
    public:

        template<typename Func>
        static IThread build(Func&& worker) {
            auto thread{ std::make_shared<Thread>(std::forward<Func>(worker)) };
            IThread _;
            _.start = [thread]() {
                thread->start();  
            };
            _.stop = [thread]() {
                thread->stop();  
            };
            _.valid = [thread]() {
                return thread->valid();
            };
            return _;
        }
    };

}