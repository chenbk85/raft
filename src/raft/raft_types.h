#ifndef RAFT_TYPES_H_NCQISTAW
#define RAFT_TYPES_H_NCQISTAW
#include <stdint.h>

namespace raft {

enum RaftState {
    StateLeader = 1,
    StateFollower,
    StateCandidate
};
    
} /* namespace raft  */ 


#endif /* end of include guard: RAFT_TYPES_H_NCQISTAW */
