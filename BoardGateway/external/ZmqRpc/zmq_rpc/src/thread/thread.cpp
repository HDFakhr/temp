#include "zmq_rpc/thread/thread.hpp"

using zmq_rpc::thread::Thread;

void Thread::start() {
    stop();
    _stop_flag.store(false);
    _future = std::async(std::launch::async, _worker);
}

void Thread::stop() {
    _stop_flag.store(true);
    if (_future.valid())
        _future.get();
}

bool Thread::valid() {
    return _future.valid();
}