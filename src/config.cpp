#include "config.h"
#include <getopt.h>
#include <iostream>
#include <regex>

namespace raft {

Config::Config():
    name("node0"),
    listen_node_addr("0.0.0.0:3421"),
    listen_client_addr("0.0.0.0:3422")
{
}

static void printUsage() {
    printf("Usage: \n");
    //TODO:
}

static bool parseNodeAddress(
        const std::string& str,
        const std::string& exclude_self_name,
        std::map<std::string, std::string>& addrs
        ) {
    std::string s = str;
    std::regex expr("([\\w]+)=([^,]+)");
    std::smatch match;

    while (std::regex_search(s, match, expr)) {
        if (match[1] != exclude_self_name) {
            auto ret = addrs.emplace(match[1], match[2]);
            if (!ret.second)
                std::cerr << "conflict node name in cmdline.(" << match[1] << ")"<< std::endl;
        }
        s = match.suffix().str();    
    }
    
    return true;
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

    if(!parseNodeAddress(cluster_addrs, name, peer_nodes)) {
        std::cerr << "Parse node address failed(invalid: " << cluster_addrs <<")" << std::endl;
        exit(EXIT_FAILURE);
    }

    std::cout << "RaftNode: " << name << " <--> " << listen_node_addr << std::endl;
    for (auto &e:peer_nodes) {
        std::cout << "RaftNode: " << e.first << " <--> " << e.second << std::endl;
    }
}
    
} /* namespace raft  */ 

