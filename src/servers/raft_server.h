#ifndef RAFT_SERVER_H_L1ER0J4I
#define RAFT_SERVER_H_L1ER0J4I

#include "proto/gen_cpp/raft.grpc.pb.h"

namespace raft {

class RaftServiceImpl: public RaftService::Service {
public:
    RaftServiceImpl();
    virtual ~RaftServiceImpl();

    virtual ::grpc::Status RequesetVote(::grpc::ServerContext* context, const ::raft::VoteRequest* request, ::raft::VoteResponse* response);
    virtual ::grpc::Status AppendEntries(::grpc::ServerContext* context, const ::raft::AppendEntriesRequest* request, ::raft::AppendEntriesResponse* response);
    virtual ::grpc::Status InstallSnapShot(::grpc::ServerContext* context, const ::raft::InstallSnapShotRequest* request, ::raft::InstallSnapShotResponse* response);
};

class RaftServer {
public:
    RaftServer();
    ~RaftServer();

    void start(const std::string& listen_addr);

private:
    RaftServiceImpl service_;
};
    
} /* namespace raft  */ 


#endif /* end of include guard: RAFT_SERVER_H_L1ER0J4I */
