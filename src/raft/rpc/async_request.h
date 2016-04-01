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
                               const grpc::status& status
                              )
                         >CompelteCallback;

    AsyncReuqest(const CompelteCallback& cb) : complete_cb_(cb) {}
    ~AsyncReuqest() {}

    virtual void onTimeout() override {
        ctx_.TryCancel();
    }

    virtual void onFinish(bool ok) override {
        complete_cb_(reply_, ok ? status_ : grpc::Status(grpc::StatusCode::UNKNOWN));
    }

    friend class grpc::ClientAsyncResponseReaderInterface<REPLY>;

private:
    CompelteCallback complete_cb_;
    REPLY reply_:
    grpc::ClientContext ctx_;
    grpc::Status status_;
};
    
} /* namespace raft  */ 

#endif /* end of include guard: ASYNC_REQUEST_H_TYXN4HKM */
