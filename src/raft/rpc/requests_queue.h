#ifndef REQUEST_QUEUES_H_VJHG1FLX
#define REQUEST_QUEUES_H_VJHG1FLX
#include <memory>
#include <mutex>
#include <unordered_map>
#include <thread>
#include <grpc++/completion_queue.h>

#include "base_request.h"

namespace raft {

class RequestsQueue {
public:
    typedef std::shared_ptr<BaseRequest> ReuqestPtr;

    RequestsQueue();
    ~RequestsQueue();

    uintptr_t push(const ReuqestPtr& r);

    grpc::CompletionQueue* cq() {
        return &cq_;
    }

private:
    void pullRoutine();
    void schedule(uintptr_t seq, bool ok);

private:
    mutable std::mutex mu_;
    std::unordered_map<uintptr_t, ReuqestPtr> requests_map_;
    uintptr_t next_seq_;

    grpc::CompletionQueue cq_;   // grpc完成队列
    std::thread pull_thr_;       // 论循grpc完成队列的线程
};
    
} /* namespace raft  */ 


#endif /* end of include guard: REQUEST_QUEUES_H_VJHG1FLX */
