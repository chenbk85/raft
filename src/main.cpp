#include <iostream>
#include <unistd.h>
#include "servers/raft_server.h"
#include "config.h"
#include "raft/raft_consensus.h"

using namespace raft;

int main(int argc, char *argv[])
{
    Config cfg;
    cfg.parseCommandLine(argc, argv);

    RaftServer server;
    server.start(cfg.listen_node_addr);

    raft::RaftConsensus rc(cfg);

    server.wait();

    return 0;
}
