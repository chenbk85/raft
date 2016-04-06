#include "raft_consensus.h"

#include <iostream>
#include <functional>
#include "support/util.h"

namespace raft {

RaftConsensus::RaftConsensus(const Config& cfg) :
    cfg_(cfg),
    id_(cfg.id),
    term_(0),        //TODO load from file
    leaderid_(0),
    votes_(0),
    vote_for_(0),
    state_(StateFollower),
    majority_(cfg.cluster_nodes.size()/2 + 1)
{
    for (auto &e : cfg.cluster_nodes)
        peers_.emplace(std::make_pair(e.first, std::get<1>(e.second)));

    rpc_stubs_.reset(new RpcStubManager(peers_));

    becomeFollower(0, 0);
}

RaftConsensus::~RaftConsensus() {
}

void RaftConsensus::handleVoteRequest(const VoteRequest& req, VoteResponse* resp) {
    std::lock_guard<std::mutex> lock(mu_);

    if (!checkTerm(req.term(), 0)) {  // term小于当前term
        resp->set_term(term_);
        resp->set_votegranted(false);
    }
    else {
        switch (state_) {
        case StateFollower:
            // TODO: check log
            if (vote_for_ == 0 || vote_for_ == req.candidateid()) {
                vote_for_ = req.candidateid();
                resp->set_term(term_);
                resp->set_votegranted(true);
            }
            else {
                resp->set_term(term_);
                resp->set_votegranted(false);
            }
            break;
        default:
            resp->set_term(term_);
            resp->set_votegranted(false);
            break;
        }
    }
}

void RaftConsensus::handleAppendEntriesRequeset(const AppendEntriesRequest& req, AppendEntriesResponse* resp) {
    std::lock_guard<std::mutex> lock(mu_);

    if (!checkTerm(req.term(), req.leaderid())) {
        resp->set_term(term_);
        resp->set_success(false);
    }
    else {
        if (StateFollower == state_ && leaderid_ == 0)
            leaderid_ = req.leaderid();
        resp->set_term(term_);
        resp->set_success(true);
    }
}

void RaftConsensus::handleInstallSnapShotRequest(const InstallSnapShotRequest& req, InstallSnapShotResponse* resp) {
    std::lock_guard<std::mutex> lock(mu_);

    if (!checkTerm(req.term(), req.leaderid())) return;
}

// src_term: 本地发起投票请求时当时本地的term
void RaftConsensus::handleVoteResponse(uint64_t src_term, const VoteResponse& resp, const grpc::Status& status) {
    if (!status.ok() && status.error_code() != grpc::StatusCode::CANCELLED) {
        std::cerr << "RequestVote RPC failed: " << status.error_message() << std::endl;
        return;
    }

    std::lock_guard<std::mutex> lock(mu_);
    if (src_term != term_) {
        std::cout << "Mismatched term in VoteResponse:" << src_term <<
            ", cur:" << term_ << std::endl;
        return;
    }
    else if (StateCandidate == state_) {
        if (resp.votegranted()) {
            ++votes_;
            if (votes_ >= majority_)
                becomeLeader();
        }
    }

    //TODO
}

void RaftConsensus::handleAppendEntriesResponse(const AppendEntriesResponse& resp, const grpc::Status& status) {
    if (!status.ok() && status.error_code() != grpc::StatusCode::CANCELLED) {
        std::cerr << "AppendEntries RPC failed: " << status.error_message() << std::endl;
        return;
    }

    std::lock_guard<std::mutex> lock(mu_);
    //TODO
}

void RaftConsensus::handleInstallSnapShotResponse(const InstallSnapShotResponse& resp, const grpc::Status& status) {
    if (!status.ok() && status.error_code() != grpc::StatusCode::CANCELLED) {
        std::cerr << "InstallSnapShot RCP failed: " << status.error_message() << std::endl;
        return;
    }

    std::lock_guard<std::mutex> lock(mu_);

}

bool RaftConsensus::checkTerm(uint64_t term, uint64_t leader) {
    if (term < term_)  {
        return false;
    }
    else if (term > term_) {
        std::cout << "Higher term found:" << term << ", cur:" << term_ <<
            ", become follower from " << stateToString(state_) << std::endl;
        becomeFollower(term, leader);
    }
    else {
        if (StateCandidate == state_) {
            std::cout << "Same term found:" << term << ", cur:" << term_ <<
                ", become follower from " << stateToString(state_) << std::endl;

            becomeFollower(term, leader);
        }
    }
    return true;
}

void RaftConsensus::becomeLeader() {
    std::cout << "RaftState: become Leader" << std::endl;

    state_ = StateLeader;
    leaderid_ = id_;

    // 删除选举定时器
    removeElectionTimer();
    // 开启心跳超时器
    resetHeartbeatTimer();

    // 给其他节点发送心跳，建立Leader权威
    AppendEntriesRequest req;
    req.set_term(term_);
    req.set_leaderid(id_);
    req.set_prevlogterm(0); // TODO:
    req.set_prevlogindex(0);
    req.set_leadercommit(0);
    for (auto& kv: peers_) {
        auto stub = rpc_stubs_->getStub(kv.first);
        if (stub) {
            stub->AppendEntriesAsync(
                    req, 
                    std::function<void(const AppendEntriesResponse&, const grpc::Status&)>(),  // TODO:无视返回值？
                    1000
                    );
        }
    }
}

void RaftConsensus::becomeFollower(uint64_t term, uint64_t leader) {
    std::cout << "RaftState: become follower" << std::endl;

    term_ = term;
    leaderid_ = leader;
    state_ = StateFollower;
    vote_for_ = 0;

    // 删除心跳超时器
    removeHeartbeatTimer();

    // 开启选举超时定时器
    resetElectionTimer();
}

void RaftConsensus::becomeCandidate() {
    std::cout << "RaftState: become candidate" << std::endl;

    state_ = StateCandidate;
    votes_ = 0;  
    
    // 删除心跳定时器
    removeHeartbeatTimer();

    // 开启选举超时定时器
    resetElectionTimer();

    ++term_;    // term + 1
    ++votes_;   // 给自己投票

    if (votes_ >= majority_)
        becomeLeader();

    VoteRequest req;
    req.set_term(term_);
    req.set_candidateid(id_); 
    req.set_lastlogterm(0);  //TODO: set logger
    req.set_lastlogindex(0);
    for (auto& kv: peers_) {
        auto stub = rpc_stubs_->getStub(kv.first);
        if (stub) {
            stub->RequesetVoteAsync(
                    req, 
                    std::bind(&RaftConsensus::handleVoteResponse, this, term_, std::placeholders::_1, std::placeholders::_2),
                    1000
                    );
        }
    }
}

void RaftConsensus::onElectionTimeout() {
    std::cout << "Election Timeout." << std::endl;

    std::lock_guard<std::mutex> lock(mu_);

    becomeCandidate();
}

void RaftConsensus::resetElectionTimer() {
    // 超时选举随机化
    int timeout_ms = randomInt(cfg_.election_timeout_ms/2, cfg_.election_timeout_ms);
    elect_timer_.reset(new ElectionTimer(this));
    timeout_queue_.push(elect_timer_, timeout_ms);
}

void RaftConsensus::removeElectionTimer() {
    elect_timer_.reset();
}

void RaftConsensus::onHeartbeatTimeout() {
    std::lock_guard<std::mutex> lock(mu_);

    std::cout << "Heartbeat timeout..." << std::endl;

    if (StateLeader != state_) {
        return;
    }

    // 发送心跳
    AppendEntriesRequest req;
    req.set_term(term_);
    req.set_leaderid(id_);
    req.set_prevlogterm(0); // TODO:
    req.set_prevlogindex(0);
    req.set_leadercommit(0);

    for (auto& kv: peers_) {
        auto stub = rpc_stubs_->getStub(kv.first);
        if (stub) {
            stub->AppendEntriesAsync(
                    req, 
                    std::function<void(const AppendEntriesResponse&, const grpc::Status&)>(),  // TODO:无视返回值？
                    1000
                    );
        }
    }

    resetHeartbeatTimer();
}

void RaftConsensus::resetHeartbeatTimer() {
    heartbeat_timer_.reset(new HeartbeatTimer(this));
    timeout_queue_.push(heartbeat_timer_, cfg_.heartbeat_timeout_ms);
}

void RaftConsensus::removeHeartbeatTimer() {
    heartbeat_timer_.reset();
}


} /* namespace raft  */ 
