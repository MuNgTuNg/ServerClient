#include <server.hpp>
namespace shb{




void Server::acceptConnection(Socket& client, int serverFD){
    int clientAddressSize = sizeof (struct sockaddr_in);
    client.FD = accept(serverFD,(sockaddr*)&client.address,(socklen_t*)&clientAddressSize);
    client.accepted = client.FD>0;

    if(!client.accepted){
        printf("Client not accepted!\n");
    }
    else{
        printf("New client connected!\n");
    }

}

void processData(int clientFD){
    char buffer[1024];

    while (true)
    {
        ssize_t  amountReceived = recv(clientFD,buffer,1024,0);

        if(amountReceived>0)
        {
            buffer[amountReceived] = 0;
            printf("%s\n",buffer);

        }

        if(amountReceived==0)
            break;
    }

    close(clientFD);
}

//create client, accept connection, push to vector and process data
void Server::run(){
    while(true){
        Socket client;
        acceptConnection(client,serverFD);
        clients.push_back(client);

        std::thread dataThread(processData,client.FD);
        dataThread.detach();
    }
}

//create server and spin it up TODO:: take in IP and Port
void serverDriverCode(){
    Server server{8091,"127.0.1.1"};
    server.run();
}

} //namespace shb