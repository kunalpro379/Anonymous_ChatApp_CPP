// Chat Server
#include"SeverConnect.hpp"

int main(int argc, char* argv[])
{
    ServerNetwork server_network(2525);
    server_network.ServerRun();
    return 0;
}