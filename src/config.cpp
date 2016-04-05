#include "config.h"
#include <getopt.h>
#include <iostream>
#include <regex>
#include <openssl/sha.h>

namespace raft {

// 由节点名字和地址生成一个唯一的id
static uint64_t newID(const std::string& name, const std::string& addr) {
    SHA_CTX ctx;
    SHA1_Init(&ctx);

    SHA1_Update(&ctx, name.c_str(), name.length());
    SHA1_Update(&ctx, addr.c_str(), addr.length());

    unsigned char hash[SHA_DIGEST_LENGTH];
    SHA1_Final(hash, &ctx);

    uint64_t id = hash[0];
    for (size_t i = 1; i < 8; ++i) {
        id = (id << 8) + hash[i];
    }

    return id;
}

// 地址格式：node1=127.0.0.1:12380,node2=127.0.0.1:22380,node3=127.0.0.1:32380
static bool parseClusterNodes(
        const std::string& str,
        const std::string& self_name,
        std::map<uint64_t, std::tuple<std::string, std::string> >& cluster_nodes
        ) {
    std::string s = str;
    std::regex expr("([\\w]+)=([^,]+)");
    std::smatch match;

    while (std::regex_search(s, match, expr)) {
        if (match[1] != self_name) {
            uint64_t id = newID(match[1], match[2]);
            auto ret = cluster_nodes.emplace(id, std::make_tuple(match[1], match[2]));
            if (!ret.second) {
                std::cerr << "conflict node id in cmdline.(" << match[1] << ")"<< std::endl;
                return false;
            }
        }
        s = match.suffix().str();    
    }

    return true;
}


Config::Config():
    name("node0"),
    id(0),
    listen_node_addr("0.0.0.0:2380"),
    listen_client_addr("0.0.0.0:2379"),
    data_dir(name + ".raftdata"),
    election_timeout_ms(1000), // TODO
    heartbeat_timeout_ms(200)  // TODO

{
    id = newID(name, listen_node_addr);
}

static void printUsage() {
    printf("Usage: \n");
    //TODO:
}

void Config::parseCommandLine(int argc, char** argv) {
    static struct option long_options[] = {
        {"name",           required_argument, 0,  0 },
        {"listen-node",    required_argument, 0,  0 },
        {"listen-client",  required_argument, 0,  0 },
        {"cluster",        required_argument, 0,  0 },
        {0,                0,                 0,  0 }
    };

    std::string cluster_addrs;

    while (true) {
        int option_index = 0;
        int c = getopt_long_only(argc, argv, "", long_options, &option_index);
        if (-1 == c) {
            break;
        }
        else if ('?' == c) {
            printUsage();
            exit(EXIT_FAILURE);
        }

        switch (option_index) {
        case 0:
            if (optarg) name = optarg;
            break;
        case 1:
            if (optarg) listen_node_addr = optarg;
            break;
        case 2:
            if (optarg) listen_client_addr = optarg;
            break;
        case 3:
            if (optarg) cluster_addrs = optarg;
            break;
        default:
            printUsage();
            exit(EXIT_FAILURE);
        }
    }

    if(!parseClusterNodes(cluster_addrs, name, cluster_nodes)) {
        std::cerr << "Parse node address failed(invalid: " << cluster_addrs <<")" << std::endl;
        exit(EXIT_FAILURE);
    }

    std::cout << "RaftNode " << name << ": " << id  << " <--> " 
        << listen_node_addr << std::endl;

    for (auto &e:cluster_nodes) {
        std::cout << "RaftNode " << std::get<0>(e.second) << ": " << e.first  << " <--> " 
            << std::get<1>(e.second) << std::endl;
    }
}
    
} /* namespace raft  */ 

