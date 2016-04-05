#ifndef RPC_STUB_H_H04CKEUK
#define RPC_STUB_H_H04CKEUK
#include "requests_queue.h"
#include "support/timer.h"
#include "proto/gen_cpp/raft.grpc.pb.h"

namespace raft {

class RpcStub {
public:
    RpcStub(uint64_t peer_id, const std::string& addr, 
            RequestsQueue& rq, TimeoutQueue& tq
            );

    ~RpcStub();

    uint64_t id() const { return id_; }
    const std::string& addr() const { return addr_; }

    void RequesetVoteAsync(
            const raft::VoteRequest& request,
            const std::function<void(const raft::VoteResponse&, const grpc::Status)>& cb,
            size_t timeout_ms
            );

    void AppendEntriesAsync(
            const raft::AppendEntriesRequest& request,
            const std::function<void(const raft::AppendEntriesResponse&, const grpc::Status)>& cb,
            size_t timeout_ms
            );

    void InstallSnapShotAsync(
            const raft::InstallSnapShotRequest& request,
            const std::function<void(const raft::InstallSnapShotResponse&, const grpc::Status)>& cb,
            size_t timeout_ms
            );

private:
    RpcStub(const RpcStub&) = delete;
    RpcStub& operator=(const RpcStub&) = delete;

private:
    const uint64_t id_;
    const std::string addr_;
    RequestsQueue& request_que_;
    TimeoutQueue& timeout_que_;
    std::unique_ptr<RaftService::Stub> stub_;
};

class RpcStubManager {
public:
    RpcStubManager(const std::map<uint64_t, std::string>& peers);
    ~RpcStubManager();

    std::shared_ptr<RpcStub> getStub(uint64_t peer_id);

private:
    typedef std::shared_ptr<RpcStub> StubPtr;

    RequestsQueue rq_;
    TimeoutQueue tq_;
    std::map<uint64_t, StubPtr> stubs_;
};
    
} /* namespace raft  */ 


#endif /* end of include guard: RPC_STUB_H_H04CKEUK */
