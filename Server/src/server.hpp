#pragma once
#include<iostream>
#include <vector>
#include <thread>
#include <assert.h>

#include <stdio.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string.h>
#include <malloc.h>
#include <unistd.h>


/*
       TODO:: 
            » Send to other clients
            » look into the processData function
            » Add IP details read from file functionality
*/

namespace shb{

struct Socket{
    int FD;
    std::string IP;
    int error;
    bool accepted;
    struct sockaddr_in address;
    std::string name;
    
};





class Server{
   public:
    Server(int portIN, const std::string& ipIN);
    void run();
    

    void acceptConnection(Socket& client, int serverFD);
    void processData(Socket client);
    void sendMessageToAllClients(char* buffer, int serverFD);
    void printClientData(Socket client);
    void takeInput();
    ~Server();

     //TODO:: make this work
    // struct serverData {
    //     char buffer[1024]; 
    // };
    
    // static serverData data;



   private:
    //take in port and IP at runtime: TODO::add reading from a file to do this
    int port = 8088;
    std::string ip = "127.0.1.1";
    int serverFD;
    struct sockaddr_in  *address = nullptr;

    
    static std::vector<Socket> clients;
    std::string input;
    
};


void serverDriverCode(int, std::string);

} //namespace shb
