#include "config.h"
#include <getopt.h>
#include <iostream>
#include <regex>

namespace raft {

// 由节点名字生成一个唯一的id
static uint64_t newID(const std::string& name) {
    //TODO:
    return 1;
}

// 
static bool parseClusterNodes(
        const std::string& str,
        const std::string& self_name,
        std::map<uint64_t, std::tuple<std::string, std::string> > cluster_nodes
        ) {
    std::string s = str;
    std::regex expr("([\\w]+)=([^,]+)");
    std::smatch match;

    while (std::regex_search(s, match, expr)) {
        if (match[1] != self_name) {
            auto ret = cluster_nodes.emplace(newID(match[1]), std::make_tuple(match[1], match[2]));
            if (!ret.second)
                std::cerr << "conflict node name in cmdline.(" << match[1] << ")"<< std::endl;
        }
        s = match.suffix().str();    
    }

    return true;
}


Config::Config():
    name("node0"),
    id(newID(name)),
    listen_node_addr("0.0.0.0:3421"),
    listen_client_addr("0.0.0.0:3422"),
    data_dir(name + ".raftdata")
{
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

    cluster_nodes.emplace(id, std::make_tuple(name, listen_node_addr));

    for (auto &e:cluster_nodes) {
        std::cout << "RaftNode " << std::get<0>(e.second) << ": " << e.first  << " <--> " 
            << std::get<1>(e.second) << std::endl;
    }
}
    
} /* namespace raft  */ 

