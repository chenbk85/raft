#ifndef RESPONSE_PULLER_H_BDJODC8S
#define RESPONSE_PULLER_H_BDJODC8S
#include <thread>
#include <grpc++/completion_queue.h>

namespace raft {

class RequestsQueue;

// 从完成队列中拉取已完成的请求
class ResponsePuller {
public:
    ResponsePuller(RequestsQueue* rq);
    ~ResponsePuller();

    grpc::CompletionQueue& cq() {
        return cq_;
    }

private:
    void pullRoutine();

private:
    RequestsQueue* rq_;
    grpc::CompletionQueue cq_;
    std::thread thr_;
};
    
} /* namespace raft  */ 



#endif /* end of include guard: RESPONSE_PULLER_H_BDJODC8S */
