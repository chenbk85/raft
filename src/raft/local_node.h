#ifndef LOCAL_NODE_H_XE5YSUG3
#define LOCAL_NODE_H_XE5YSUG3
#include "raft_types.h"

namespace raft {

class LocalNode {
public:
    LocalNode();
    ~LocalNode();

private:
    uint64_t leader_;       // leader的id
    RaftState state_;       // 状态、身份
};
    
} /* namespace raft */ 


#endif /* end of include guard: LOCAL_NODE_H_XE5YSUG3 */
