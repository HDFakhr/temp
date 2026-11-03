#pragma once

#include <future>
#include <functional>
#include <atomic>

namespace zmq_rpc::thread
{
    class Thread {
    public:
        using worker_t = std::function<void()>;

        template<typename Func>
        Thread(Func worker) {
            _worker = [&, w = std::forward<Func>(worker)]() {
                while (!_stop_flag.load())
                    w();
            };
        }

        ~Thread() {
            stop();
        }
            
        void start();
        void stop();
        bool valid();
        
    private:
        std::atomic_bool _stop_flag{ false };
        worker_t _worker;
        std::future<void> _future;
    };
}