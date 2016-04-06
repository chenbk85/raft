#ifndef RAFT_CONSENSUS_H_UEDTFRIC
#define RAFT_CONSENSUS_H_UEDTFRIC
#include "raft/rpc/rpc_stub.h"
#include "config.h"
#include "raft/raft_types.h"
#include "support/timer.h"

namespace raft {

class RaftConsensus {
public:
    RaftConsensus(const Config& cfg);
    ~RaftConsensus();

public:
    void handleVoteRequest(const VoteRequest& req, VoteResponse* resp);
    void handleAppendEntriesRequeset(const AppendEntriesRequest& req, AppendEntriesResponse* resp);
    void handleInstallSnapShotRequest(const InstallSnapShotRequest& req, InstallSnapShotResponse* resp);

private:
    void handleVoteResponse(uint64_t src_term, const VoteResponse& resp, const grpc::Status& status);
    void handleAppendEntriesResponse(const AppendEntriesResponse& resp, const grpc::Status& status);
    void handleInstallSnapShotResponse(const InstallSnapShotResponse& resp, const grpc::Status& status);

    bool checkTerm(uint64_t term, uint64_t leader);

    void becomeLeader();
    void becomeFollower(uint64_t term, uint64_t leader);
    void becomeCandidate();

private:
    struct ElectionTimer : public Timeoutable {
    public:
        ElectionTimer(RaftConsensus *parent) : parent_(parent) {}
        virtual void onTimeout() override {
            parent_->onElectionTimeout();
        }

    private:
        RaftConsensus* parent_;
    };

    struct HeartbeatTimer : public Timeoutable {
        HeartbeatTimer(RaftConsensus *parent) : parent_(parent) {}
        virtual void onTimeout() override {
            parent_->onHeartbeatTimeout();
        }

        private:
        RaftConsensus* parent_;
    };


    void onElectionTimeout();
    void resetElectionTimer();
    void removeElectionTimer();

    void onHeartbeatTimeout();
    void resetHeartbeatTimer();
    void removeHeartbeatTimer();

private:
    const Config& cfg_;
    std::map<uint64_t, std::string> peers_;
    std::unique_ptr<RpcStubManager> rpc_stubs_;

    std::mutex mu_;
    uint64_t id_;
    uint64_t term_;
    uint64_t leaderid_;
    int votes_;
    uint64_t vote_for_;
    RaftState state_;
    const int majority_;

    std::shared_ptr<ElectionTimer> elect_timer_;
    std::shared_ptr<HeartbeatTimer> heartbeat_timer_;
    TimeoutQueue timeout_queue_;
};
    
} /* namespace raft  */ 



#endif /* end of include guard: RAFT_CONSENSUS_H_UEDTFRIC */
