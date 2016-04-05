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

private:
    void becomeLeader();
    void becomeFollower();
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

    void onElectionTimeout();
    void resetElectionTimer();

private:
    const Config& cfg_;
    std::map<uint64_t, std::string> peers_;
    std::unique_ptr<RpcStubManager> rpc_stubs_;

    std::mutex mu_;
    uint64_t id_;
    uint64_t term_;
    RaftState state_;
    const int half_nodes_;


    std::shared_ptr<ElectionTimer> elect_timer_;
    TimeoutQueue timeout_queue_;
};
    
} /* namespace raft  */ 



#endif /* end of include guard: RAFT_CONSENSUS_H_UEDTFRIC */
