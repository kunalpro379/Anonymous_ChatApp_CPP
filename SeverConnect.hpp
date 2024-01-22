#pragma once
#pragma once
#include <iostream>
#include <thread>
#include <vector>
#include <chrono>
#include <string.h>
#include <SFML/Network.hpp>
using namespace std;
#define MAX_RAW_DATA 256 // Max bytes supported on Raw Data mode

#define coutendl(x) std::cout << x << std::endl
#define cout(x) std::cout << x

class ServerNetwork {

	sf::TcpListener listner;
	vector<sf::TcpSocket*>client_array;
	unsigned short listen_port;

public:

    ServerNetwork(unsigned short);
    void ConnectClients(std::vector<sf::TcpSocket*>*);
    void DisconnectClient(sf::TcpSocket*, size_t);

    void ReceivePacket(sf::TcpSocket*, size_t);

    void BroadcastPacket(sf::Packet&, sf::IpAddress, unsigned short);

    void ManagePackets();
    
	void ServerRun();


};
