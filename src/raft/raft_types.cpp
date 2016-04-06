#include "raft_types.h"

namespace raft {

std::string stateToString(RaftState state) {
    switch (state) {
        case StateLeader:
            return "Leader";
        case StateFollower:
            return "Follower";
        case StateCandidate:
            return "Candidate";
        default:
            return "Invalid";
    }
}

} /* namespace raft  */ 
