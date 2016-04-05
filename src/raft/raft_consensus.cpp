#include "raft_consensus.h"

#include <iostream>
#include "support/util.h"


namespace raft {

RaftConsensus::RaftConsensus(const Config& cfg) :
    cfg_(cfg),
    id_(cfg.id),
    term_(0),        //TODO load from file
    state_(StateFollower),
    half_nodes_((cfg.cluster_nodes.size() + 1)/2)
{
    for (auto &e : cfg.cluster_nodes)
        peers_.emplace(std::make_pair(e.first, std::get<1>(e.second)));

    rpc_stubs_.reset(new RpcStubManager(peers_));

    becomeFollower();
}

RaftConsensus::~RaftConsensus() {
}

void RaftConsensus::becomeLeader() {
}

void RaftConsensus::becomeFollower() {
    std::lock_guard<std::mutex> lock(mu_);

    std::cout << "RaftState: become follower" << std::endl;
    resetElectionTimer();
}

void RaftConsensus::becomeCandidate() {
    std::lock_guard<std::mutex> lock(mu_);

    std::cout << "RaftState: become candidate" << std::endl;
}

void RaftConsensus::resetElectionTimer() {
    int timeout_ms = randomInt(cfg_.election_timeout_ms/2, cfg_.election_timeout_ms);
    elect_timer_.reset(new ElectionTimer(this));
    timeout_queue_.push(elect_timer_, timeout_ms);
}

void RaftConsensus::onElectionTimeout() {
    becomeCandidate();
}

} /* namespace raft  */ 
