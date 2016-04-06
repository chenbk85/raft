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

    raft::RaftConsensus rc(cfg);

    RaftServer server(cfg.listen_node_addr, rc);
    server.waitForExit();

    return 0;
}
