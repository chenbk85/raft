#include "requests_queue.h"
#include <iostream>

namespace raft {

RequestsQueue::RequestsQueue() :
    next_seq_(1),
    pull_thr_(std::bind(&RequestsQueue::pullRoutine, this))
{
}

RequestsQueue::~RequestsQueue() {
    //TODO: check remain
    cq_.Shutdown();
    pull_thr_.join();
}

uintptr_t RequestsQueue::push(const ReuqestPtr& r) {
    std::lock_guard<std::mutex> guard(mu_);

    requests_map_.emplace(next_seq_, r);
    auto ret = next_seq_;
    ++next_seq_;
    return ret;
}

void RequestsQueue::schedule(uintptr_t seq, bool ok) {
    ReuqestPtr req;

    {
        std::lock_guard<std::mutex> guard(mu_);
        auto it = requests_map_.find(seq);
        if (it != requests_map_.end()) {
            req = it->second;
            requests_map_.erase(it);
        }
    }

    if (req) {
        try {
            req->onFinish(ok);
        }
        catch (...) {
            //TODO
        }
    }
}

void RequestsQueue::pullRoutine() {
    for ( ; ; ) {
        void *tag = nullptr;
        bool ok = false;
        bool ret = cq_.Next(&tag, &ok);
        if (ret) {
            schedule(reinterpret_cast<uintptr_t>(tag), ok);
        }
        else {    //SHUTDOWN
            std::cout << "Response puller routine end." << std::endl;
            return;
        }
    }
}
    
} /* namespace raft  */ 
