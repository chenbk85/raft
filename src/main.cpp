#include <iostream>
#include "servers/raft_server.h"

using namespace raft;

int main(int argc, char *argv[])
{
    std::unique_ptr<RaftServer> raft_server(new RaftServer);
    raft_server->start("0.0.0.0:24002");

    return 0;
}
