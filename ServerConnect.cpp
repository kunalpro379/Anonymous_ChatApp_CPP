#include "SeverConnect.hpp"
#include<vector>
ServerNetwork::ServerNetwork(unsigned short port) :listen_port(port)
{
    coutendl("ChatSy Server Started ");//listen on specified port
    if (listner.listen(listen_port) != sf::Socket::Done) {
        coutendl("Could not listen ! ");
    }

}

void ServerNetwork::ConnectClients(vector<sf::TcpSocket*>* client_array) // pointer to the listner socket
{
    while (true) {
        //create a neuw  TcpSocket for new client
        sf::TcpSocket* new_client = new sf::TcpSocket;
        //new Tcpsocke t object is dynamically allocated so we need to delete it later 
        //each connected client needs its own socket
        if (listner.accept(*new_client) != sf::Socket::Done) { //accepting the incoming client connection - if done
            new_client->setBlocking(false); //newclient socket is set to non_blocking mode,,, allowing it to handle multiple clients at once
            client_array->push_back(new_client); //add the new client to the vector of clients
            coutendl("Client Connected ! ");
            coutendl("Added_Client" << new_client->getRemoteAddress() \
                << "||" << new_client->getRemotePort() << "on slot" << client_array->size() - 1);
        }
        else {
            coutendl("ServerListner error, restart the server ");
            delete(new_client);
            break;
        }

    }
}

void ServerNetwork::DisconnectClient(sf::TcpSocket* socket_pointer, size_t position) {
    coutendl("client" << socket_pointer->getRemoteAddress()   //getting the address of the client to be disconnected
        << "|||" << socket_pointer->getRemotePort() << "on slot" << position << "disconnected");
    socket_pointer->disconnect(); //disconnecting the client
    //this method is called on tcpsocket object pointed by socket_pointer- disconnects the socket from its remote peer
    delete(socket_pointer); //deleting the socket object_ to prevent memory leaks
    client_array.erase(client_array.begin() + position); //removing the client from the vector of clients at specific position

}


void ServerNetwork::BroadcastPacket(sf::Packet& packet, sf::IpAddress exclude_address, unsigned short port)
{//iterate through each client in the client_array
    for (size_t iterator = 0;iterator < client_array.size();iterator++) {
        //pointer to the current client
        sf::TcpSocket* client = client_array[iterator];
        //if the client is not the one to be excluded
        if (client->getRemoteAddress() != exclude_address || client->getRemotePort() != port) {

            //if current client is not excluded , sen d the packet to it 
            if (client->send(packet) != sf::Socket::Done) {
                coutendl("Error sending packet to client" << client->getRemoteAddress());
            }
        }
    }
}

void ServerNetwork::ReceivePacket(sf::TcpSocket* client, size_t iterator) {

    //creating an packet to store the reicieved data

    sf::Packet packet;

    // recieving the packet from the client
    if (client->receive(packet) == sf::Socket::Disconnected) {
        //IF the client is disconnected
        //DISCONNECT THE CLIENT AND EXIT
        DisconnectClient(client, iterator);
        return;
    }
    //cheak if recieved packet contains data
    if (packet.getDataSize() > 0) {
        string reicieved_data;
        packet >> reicieved_data; //extracting the data from the packet
        //clear the packet for reuse
        packet.clear();  //clearing for reuse 
        packet << reicieved_data << client->getRemoteAddress().toString() << client->getRemotePort();
        //Brodcasting the modified socket to all the clients remote addr and the port to the packet 
        BroadcastPacket(packet, client->getRemoteAddress(), client->getRemotePort());
        coutendl(client->getRemoteAddress().toString() << ":|:" << client->getRemotePort() << "||" << reicieved_data);

    }
}
void ServerNetwork::ManagePackets()
{

    while (true)
    {
        for (size_t iterator = 0; iterator < client_array.size(); iterator++)
        {
            ReceivePacket(client_array[iterator], iterator);
        }

        std::this_thread::sleep_for((std::chrono::milliseconds)250);
    }

}

void ServerNetwork::ServerRun()
{
    std::thread connection_thread(&ServerNetwork::ConnectClients, this, &client_array);

    // Wait for the connection thread to finish
    connection_thread.join();

    // Start managing packets after connecting clients
    ManagePackets();
}
