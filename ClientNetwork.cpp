#include "clientnetwork.hpp"

ClientNetwork::ClientNetwork()
{
    printn("Chat Client Started");
}

void ClientNetwork::Connect(const char* address, unsigned short port)
{
    if (socket.connect(address, port) != sf::Socket::Done)
    {
        printn("Could not connect to the server\n");
    }
    else
    {
        isConnected = true;
        printn("Connected to the server\n");
    }
}

void ClientNetwork::RecievePackets(sf::TcpSocket* socket)
{
    while (true)
    {
        if (socket->receive(last_packet) == sf::Socket::Done)
        {
            std::string received_string;
            std::string sender_address;
            unsigned short sender_port;
            last_packet >> received_string >> sender_address >> sender_port;
            printn("From (" << sender_address << ":" << sender_port << "): " << received_string);
        }

        std::this_thread::sleep_for((std::chrono::milliseconds)250);
    }
}

void ClientNetwork::SendPacket(sf::Packet& packet)
{
    if (packet.getDataSize() > 0 && socket.send(packet) != sf::Socket::Done)
    {
        printn("Could not send packet");
    }
}

void ClientNetwork::Run()
{
    //create a new thread for recieving packets asynchronously
    std::thread reception_thread(&ClientNetwork::RecievePackets, this, &socket);
    //for sednding user input to the server
    while (true) {
        if (isConnected) {
            string user_ip;
            getline(cin, user_ip);



            if (user_ip.length() < 1) { continue; }

            sf::Packet reply_packet;
            reply_packet << user_ip;

            //send the packet to the user
            SendPacket(reply_packet);
        }
    }
}