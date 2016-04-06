#ifndef RAFT_TYPES_H_NCQISTAW
#define RAFT_TYPES_H_NCQISTAW
#include <stdint.h>
#include <string>

namespace raft {

enum RaftState {
    StateLeader = 1,
    StateFollower,
    StateCandidate
};

std::string stateToString(RaftState state);

} /* namespace raft  */ 


#endif /* end of include guard: RAFT_TYPES_H_NCQISTAW */
