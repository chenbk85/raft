#include "local_node.h"

namespace raft {

LocalNode::LocalNode():
    leader(0),
    state(StateFollower),
    term(0),
    votes(0)
{
}

} /* namespace raft  */ 
