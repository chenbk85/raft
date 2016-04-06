#ifndef LOCAL_NODE_H_XE5YSUG3
#define LOCAL_NODE_H_XE5YSUG3
#include "raft_types.h"

namespace raft {

class LocalNode {
public:
    LocalNode();

    uint64_t leader;       // leader的id
    RaftState state;       // 状态、身份
    uint64_t term;         // 当前任期
    unsigned votes;          // 收到的
};
    
} /* namespace raft */ 


#endif /* end of include guard: LOCAL_NODE_H_XE5YSUG3 */
