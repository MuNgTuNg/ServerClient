#include <server.hpp>
namespace shb{

std::vector<Socket> Server::clients;

Server::Server(int portIN, const std::string& ipIN) : port(portIN),ip(ipIN) {
    printf("Server spinning up...\n");
    //sock stream connection
    serverFD = socket(AF_INET,SOCK_STREAM,0);

    //set up the address
    address = (struct sockaddr_in*)malloc(sizeof(struct sockaddr_in));
    address->sin_family = AF_INET;
    address->sin_port = htons(port);

    if(strlen(ip.c_str()) ==0) //if no IP chosen just use any
        address->sin_addr.s_addr = INADDR_ANY;
    else //convert IP address to binary
        inet_pton(AF_INET,ip.c_str(),&address->sin_addr.s_addr);
    
    //bind socket to the address
    int res = bind(serverFD,(const sockaddr*)address,sizeof(*address));
    
    if(res != 0)
        printf("Server bind unsuccessful\n");
    
    //mark this socket as a passive socket, that waits for incoming connections
    res = listen(serverFD,10);

    printf("Server running\n\n");
    
}



void Server::acceptConnection(Socket& client, int serverFD){
    printf("Waiting on connections...\n");
    int clientAddressSize = sizeof (struct sockaddr_in);
    client.FD = accept(serverFD,(sockaddr*)&client.address,(socklen_t*)&clientAddressSize);
    client.accepted = client.FD>0;

    if(!client.accepted){
        printf("Client not accepted!\n");
    }
    else{
        std::cout << "\n--New client connected!--\n\n";
    }

}

void Server::sendMessageToAllClients(char* buffer, int socketFD){
    for(int i = 0; i < clients.size(); ++i){
        if(clients[i].FD != socketFD){
            ssize_t amountWasSent =  send(clients[i].FD,buffer, strlen(buffer), 0);
        }
    }
}


void Server::printClientData(Socket client){
    //convert IP to string
    char buffer[INET_ADDRSTRLEN];
    inet_ntop( AF_INET, &client.address.sin_addr, buffer, sizeof( buffer ));

    std::cout << "Client details:\n";
    std::cout << "Client name: " << client.name << "\n";
    std::cout << "Client IP: " << buffer << "\n";
    std::cout << "\n";
}


void Server::takeInput(){
    while(true){
        std::cin >> input;
        if(input == "test"){
            sendMessageToAllClients("test", -1);
        }
    }
    return;
}

void Server::processData(Socket client){
    char buffer[1024]; 

    //get name from client and store it locally
    ssize_t  amountReceived = recv(client.FD,buffer,1024,0);
    if(amountReceived > 0){
        buffer[amountReceived] = 0;
        client.name = buffer;
        printClientData(client);

        //notify users when someone's joined
        std::string msg = client.name + " joined";
        char* msgcstr = &msg[0];
        sendMessageToAllClients(msgcstr, client.FD);
    }
    
    //clients kept seperately for multithreading purposes (i think)
    for(int i = 0; i < clients.size(); ++i){
        if(clients[i].FD == client.FD){
            clients[i].name = client.name;
        }
    }
    
    
    while (true)
    {

        
        //recv is a blocking call so it needs to be on a new thread , like being on a second phone
        ssize_t  amountReceived = recv(client.FD,buffer,1024,0);
        
        //if we've actually recieved a message
        if(amountReceived>0)
        {
            buffer[amountReceived] = 0;
            printf("%s\n",buffer);
            sendMessageToAllClients(buffer,client.FD);

        }
        
        //if client has sent a 0 byte packet or disconnected, break then close the server side
        //client socket
        if(amountReceived==0)
            break;
    }
    
    close(client.FD);
    
    for(int i = 0; i < clients.size(); ++i){
        if(clients[i].FD == client.FD){
            clients.erase(clients.begin()+i);
            std::cout << "Client " << client.name << " Left\n";
        }
    }
}

//create client, accept connection, push to vector and process data
void Server::run(){
    while(true){
        std::thread inputThread(&Server::takeInput,this);
        inputThread.detach();
        Socket client;
        acceptConnection(client,serverFD);
        clients.push_back(client);

        std::thread dataThread(&Server::processData,this,client);
        dataThread.detach();

        if(input == "quit"){
            break;
        }
    }
}

//create server and spin it up TODO:: take in IP and Port
void serverDriverCode(){
    Server server{8093,"127.0.1.1"};
    server.run();
}

Server::~Server() { 
    shutdown(serverFD,SHUT_RDWR);
    delete address;
}

} //namespace shb