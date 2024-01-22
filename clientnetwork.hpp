#pragma once

#include<iostream>
#include<thread>
#include<chrono>
#include<SFML/Network.hpp>

using namespace std;
#define printn(x) cout<<x<<endl;
#define print(x) cout<<x;

class ClientNetwork {
	sf::TcpSocket socket;
	sf::Packet last_packet;

	bool isConnected = false;

public:

	ClientNetwork();
	void Connect(const char*, unsigned short);
	void RecievePackets(sf::TcpSocket*);
	void SendPacket(sf::Packet&);

	void Run();


};
