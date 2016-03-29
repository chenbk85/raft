#include <iostream>
#include "servers/raft_server.h"
#include "config.h"

using namespace raft;

int main(int argc, char *argv[])
{
    Config cfg;
    cfg.parseCommandLine(argc, argv);

    std::unique_ptr<RaftServer> raft_server(new RaftServer);
    raft_server->start(cfg.listen_node_addr);

    return 0;
}
