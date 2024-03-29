#ifndef CONFIG_H_O9CG3Z6Y
#define CONFIG_H_O9CG3Z6Y
#include <string>
#include <map>

namespace raft {

struct Config {
    std::string name;                   // human-readable raft node name, unique in cluster
    uint64_t id;                        // raft node id
    std::string listen_node_addr;       // listen other raft node incoming
    std::string listen_client_addr;     // listen client request incoming
    std::map<uint64_t, std::tuple<std::string, std::string> > cluster_nodes;   // other nodes address. key: node id, value: <name, addr>
    std::string data_dir;
    size_t election_timeout_ms;     // TODO
    size_t heartbeat_timeout_ms;    // TODO

    Config();
    void parseCommandLine(int argc, char** argv);
};
    
} /* namespace raft  */ 

#endif /* end of include guard: CONFIG_H_O9CG3Z6Y */
