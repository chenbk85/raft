#include "rpc_stub.h"
#include "async_request.h"
#include <iostream>

namespace raft {

RpcStub::RpcStub(uint64_t peer_id, const std::string& addr, RequestsQueue& rq, TimeoutQueue& tq):
    id_(peer_id),
    addr_(addr),
    request_que_(rq),
    timeout_que_(tq)
{
    auto ch = grpc::CreateChannel(addr, grpc::InsecureChannelCredentials());
    stub_ = RaftService::NewStub(ch);
}

RpcStub::~RpcStub() {
}

void RpcStub::RequesetVoteAsync(
        const raft::VoteRequest& request,
        const std::function<void(const raft::VoteResponse&, const grpc::Status)>& cb,
        size_t timeout_ms) 
{
    auto ar = AsyncReuqest<raft::VoteResponse>::newReuqest(cb);
    auto seq = request_que_.push(ar);
    timeout_que_.push(ar, timeout_ms);

    std::unique_ptr<grpc::ClientAsyncResponseReader<VoteResponse> > rpc(
            stub_->AsyncRequesetVote(ar->ctx(), request, request_que_.cq()));

    rpc->Finish(ar->reply(), ar->status(), reinterpret_cast<void*>(seq));
}

void RpcStub::AppendEntriesAsync(
        const raft::AppendEntriesRequest& request,
        const std::function<void(const raft::AppendEntriesResponse&, const grpc::Status)>& cb,
        size_t timeout_ms)
{
    auto ar = AsyncReuqest<raft::AppendEntriesResponse>::newReuqest(cb);
    auto seq = request_que_.push(ar);
    timeout_que_.push(ar, timeout_ms);

    std::unique_ptr<grpc::ClientAsyncResponseReader<AppendEntriesResponse> > rpc(
            stub_->AsyncAppendEntries(ar->ctx(), request, request_que_.cq()));

    rpc->Finish(ar->reply(), ar->status(), reinterpret_cast<void*>(seq));
}

void RpcStub::InstallSnapShotAsync(
        const raft::InstallSnapShotRequest& request,
        const std::function<void(const raft::InstallSnapShotResponse&, const grpc::Status)>& cb,
        size_t timeout_ms)
{
    auto ar = AsyncReuqest<raft::InstallSnapShotResponse>::newReuqest(cb);
    auto seq = request_que_.push(ar);
    timeout_que_.push(ar, timeout_ms);

    std::unique_ptr<grpc::ClientAsyncResponseReader<InstallSnapShotResponse> > rpc(
            stub_->AsyncInstallSnapShot(ar->ctx(), request, request_que_.cq()));

    rpc->Finish(ar->reply(), ar->status(), reinterpret_cast<void*>(seq));
}



RpcStubManager::RpcStubManager(const std::map<uint64_t, std::string>& peers) {
    for (auto& kv: peers) {
        StubPtr sp_stub(new RpcStub(kv.first, kv.second, rq_, tq_));
        stubs_.emplace(std::make_pair(kv.first, sp_stub));
        std::cout << "RpcStub to " << kv.second << "(ID:" << kv.first << ") added." << std::endl;
    }
}

RpcStubManager::~RpcStubManager() {
}

std::shared_ptr<RpcStub> RpcStubManager::getStub(uint64_t peer_id) {
    return stubs_.at(peer_id);
}

} /* namespace raft  */ 
