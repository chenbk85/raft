#include "raft_server.h"
#include <grpc++/grpc++.h>

namespace raft {
    
RaftServiceImpl::RaftServiceImpl() {
}

RaftServiceImpl::~RaftServiceImpl() {
}

::grpc::Status RaftServiceImpl::RequesetVote(
        ::grpc::ServerContext* context, 
        const ::raft::VoteRequest* request, 
        ::raft::VoteResponse* response) {
    //TODO:
    (void)context; (void)request; (void)response;
    return grpc::Status(grpc::StatusCode::UNIMPLEMENTED, "not implemented");
}

::grpc::Status RaftServiceImpl::AppendEntries(
        ::grpc::ServerContext* context, 
        const ::raft::AppendEntriesRequest* request, 
        ::raft::AppendEntriesResponse* response) {
    //TODO:
    (void)context; (void)request; (void)response;
    return grpc::Status(grpc::StatusCode::UNIMPLEMENTED, "not implemented");
}

::grpc::Status RaftServiceImpl::InstallSnapShot(
        ::grpc::ServerContext* context, 
        const ::raft::InstallSnapShotRequest* request, 
        ::raft::InstallSnapShotResponse* response) {
    //TODO:
    (void)context; (void)request; (void)response;
    return grpc::Status(grpc::StatusCode::UNIMPLEMENTED, "not implemented");
}


RaftServer::RaftServer() {
}

RaftServer::~RaftServer() {
}

void RaftServer::start(const std::string& listen_addr) {
    grpc::ServerBuilder builder;
    builder.AddListeningPort(listen_addr, grpc::InsecureServerCredentials());
    builder.RegisterService(&service_);

    std::unique_ptr<grpc::Server> server(builder.BuildAndStart());
    server->Wait();
}


} /* namespace raft  */ 
