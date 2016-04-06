#ifndef ASYNC_REQUEST_H_TYXN4HKM
#define ASYNC_REQUEST_H_TYXN4HKM
#include <grpc++/grpc++.h>
#include "base_request.h"
#include "support/timer.h"

namespace raft {

template <class REPLY>
class AsyncReuqest : public BaseRequest, public Timeoutable {
public:
    typedef std::function<void(const REPLY&, 
                               const grpc::Status& status
                              )
                         >CompelteCallback;

    AsyncReuqest(const CompelteCallback& cb) : complete_cb_(cb) {}
    ~AsyncReuqest() {}

    static std::shared_ptr<AsyncReuqest> newReuqest(const CompelteCallback& cb) {
        return std::shared_ptr<AsyncReuqest>(new AsyncReuqest(cb));
    }

    virtual void onTimeout() override {
        ctx_.TryCancel();
    }

    virtual void onFinish(bool ok) override {
        if (complete_cb_)
            complete_cb_(reply_, ok ? status_ : grpc::Status(grpc::StatusCode::UNKNOWN, ""));
    }


private:
    REPLY* reply() { return &reply_; }
    grpc::ClientContext* ctx() { return &ctx_; }
    grpc::Status* status() { return &status_; }

    friend class RpcStub;

private:
    CompelteCallback complete_cb_;
    REPLY reply_;
    grpc::ClientContext ctx_;
    grpc::Status status_;
};
    
} /* namespace raft  */ 

#endif /* end of include guard: ASYNC_REQUEST_H_TYXN4HKM */
