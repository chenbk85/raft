#include "requests_queue.h"

namespace raft {

RequestsQueue::RequestsQueue() :
    next_seq_(1),
    puller_(this)
{}

RequestsQueue::~RequestsQueue() {
    //TODO: check remain
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

    if (req)
        req->onFinish(ok);
}
    
} /* namespace raft  */ 
