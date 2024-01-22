//Chat Client
#include "ClientNetwork.hpp"

int RunClient(int argc, char* argv[]) {
    ClientNetwork client_network;
    client_network.Connect("localHost", 2525);
    client_network.Run();
    return 0;
}
