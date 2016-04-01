#ifndef REQUEST_QUEUES_H_VJHG1FLX
#define REQUEST_QUEUES_H_VJHG1FLX
#include <memory>
#include <mutex>
#include <unordered_map>
#include "base_request.h"
#include "response_puller.h"

namespace raft {

class RequestsQueue {
public:
    typedef std::shared_ptr<BaseRequest> ReuqestPtr;

    RequestsQueue();
    ~RequestsQueue();

    uintptr_t push(const ReuqestPtr& r);
    void schedule(uintptr_t seq, bool ok);

private:
    mutable std::mutex mu_;
    std::unordered_map<uintptr_t, ReuqestPtr> requests_map_;
    uintptr_t next_seq_;

    ResponsePuller puller_;
};
    
} /* namespace raft  */ 


#endif /* end of include guard: REQUEST_QUEUES_H_VJHG1FLX */
