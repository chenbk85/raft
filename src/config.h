#ifndef CONFIG_H_O9CG3Z6Y
#define CONFIG_H_O9CG3Z6Y
#include <string>
#include <map>

namespace raft {

struct Config {
    std::string name;                   // raft node name, unique in cluster
    std::string listen_node_addr;       // listen other raft node incoming
    std::string listen_client_addr;     // listen client request incoming
    std::map<std::string, std::string> peer_nodes;   // other nodes address. key: node name, value: address

    Config();
    void parseCommandLine(int argc, char** argv);
};
    
} /* namespace raft  */ 

#endif /* end of include guard: CONFIG_H_O9CG3Z6Y */
