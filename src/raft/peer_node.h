#ifndef PEER_NODE_H_CPVNWEYR
#define PEER_NODE_H_CPVNWEYR

namespace raft {

class PeerNode {
public:
    PeerNode(uint64_t id);
    ~PeerNode();

private:
    uint64_t id_;              // 节点id
    uint64_t match_index_;     // 已知的已复制到该节点的日志最高index
    uint64_t next_index_;      // 下一个将要发送给该节点的日志id
};
    
} /* namespace raft  */ 


#endif /* end of include guard: PEER_NODE_H_CPVNWEYR */
