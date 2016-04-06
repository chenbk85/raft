#ifndef RAFT_SERVER_H_L1ER0J4I
#define RAFT_SERVER_H_L1ER0J4I

#include <thread>
#include "proto/gen_cpp/raft.grpc.pb.h"

namespace raft {

class RaftConsensus;

class RaftServiceImpl: public RaftService::Service {
public:
    RaftServiceImpl(RaftConsensus& rc);
    virtual ~RaftServiceImpl();

    virtual ::grpc::Status RequesetVote(::grpc::ServerContext* context, const ::raft::VoteRequest* request, ::raft::VoteResponse* response) override;
    virtual ::grpc::Status AppendEntries(::grpc::ServerContext* context, const ::raft::AppendEntriesRequest* request, ::raft::AppendEntriesResponse* response) override;
    virtual ::grpc::Status InstallSnapShot(::grpc::ServerContext* context, const ::raft::InstallSnapShotRequest* request, ::raft::InstallSnapShotResponse* response) override;

private:
    RaftConsensus& raft_consensus_;
};

class RaftServer {
public:
    RaftServer(const std::string& listen_addr, RaftConsensus& rc);
    ~RaftServer();

    void waitForExit();

private:
    RaftServer(const RaftServer&) = delete;
    RaftServer& operator=(const RaftServer) = delete;

private:
    RaftServiceImpl service_impl_;
    std::unique_ptr<grpc::Server> server_;
};
    
} /* namespace raft  */ 


#endif /* end of include guard: RAFT_SERVER_H_L1ER0J4I */
