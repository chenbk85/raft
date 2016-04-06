#include "raft_server.h"
#include "raft/raft_consensus.h"

#include <iostream>
#include <grpc++/grpc++.h>

namespace raft {
    
RaftServiceImpl::RaftServiceImpl(RaftConsensus& rc) :
    raft_consensus_(rc) {
}

RaftServiceImpl::~RaftServiceImpl() {
}

::grpc::Status RaftServiceImpl::RequesetVote(
        ::grpc::ServerContext* context, 
        const ::raft::VoteRequest* request, 
        ::raft::VoteResponse* response) {
    (void)context;
    raft_consensus_.handleVoteRequest(*request, response);
    return grpc::Status::OK;
}

::grpc::Status RaftServiceImpl::AppendEntries(
        ::grpc::ServerContext* context, 
        const ::raft::AppendEntriesRequest* request, 
        ::raft::AppendEntriesResponse* response) {
    (void)context; 
    raft_consensus_.handleAppendEntriesRequeset(*request, response);
    return grpc::Status::OK;
}

::grpc::Status RaftServiceImpl::InstallSnapShot(
        ::grpc::ServerContext* context, 
        const ::raft::InstallSnapShotRequest* request, 
        ::raft::InstallSnapShotResponse* response) {
    (void)context;
    raft_consensus_.handleInstallSnapShotRequest(*request, response);
    return grpc::Status::OK;
}


RaftServer::RaftServer(const std::string& listen_addr, RaftConsensus& rc) :
    service_impl_(rc)
{
    grpc::ServerBuilder builder;
    builder.AddListeningPort(listen_addr, grpc::InsecureServerCredentials());
    builder.RegisterService(&service_impl_);

    //TODO: exist if address already in use
    server_ = builder.BuildAndStart();

    std::cout << "RaftServer start listen on " << listen_addr << std::endl;
}

RaftServer::~RaftServer() {
}

void RaftServer::waitForExit() {
    server_->Wait();
}

} /* namespace raft  */ 
