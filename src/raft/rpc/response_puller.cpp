#include "response_puller.h"

#include <iostream>
#include <functional>
#include "requests_queue.h"

namespace raft {
    
ResponsePuller::ResponsePuller(RequestsQueue* rq) :
    rq_(rq),
    thr_(std::bind(&ResponsePuller::pullRoutine, this))
{}

ResponsePuller::~ResponsePuller() {
    cq_.Shutdown();
    thr_.join();
}

void ResponsePuller::pullRoutine() {
    for ( ; ; ) {
        void *tag = nullptr;
        bool ok = false;
        bool ret = cq_.Next(&tag, &ok);
        if (ret) {
            rq_->schedule(reinterpret_cast<uintptr_t>(tag), ok);
        }
        else {    //SHUTDOWN
            std::cout << "Response puller routine end." << std::endl;
            return;
        }
    }
}

} /* namespace raft  */ 
