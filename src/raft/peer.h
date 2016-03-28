#ifndef RAFT_PEER_H_
#define RAFT_PEER_H_

namespace raft {

struct Peer {
    uint64_t id;
    std::string addr;
};

class PeerManager {
public:
    Peers();
    ~Peers();

private:
    /* data */
};

    
} /* namespace raft  */ 


#endif /* end of include guard: RAFT_PEER_H_ */
