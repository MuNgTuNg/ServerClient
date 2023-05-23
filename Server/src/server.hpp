#pragma once
#include<iostream>
#include <vector>
#include <thread>

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
    
};


class Server{
   public:
    Server(int portIN, const std::string& ipIN) : port(portIN),ip(ipIN) {
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
        
    }

    void run();

    void acceptConnection(Socket& client, int serverFD);

    ~Server() { 
        shutdown(serverFD,SHUT_RDWR);
        delete address;
    }

    int fd() const { return serverFD; }

   private:
    //take in port and IP at runtime: TODO::add reading from a file to do this
    int port = 8088;
    std::string ip = "127.0.1.1";
    int serverFD;
    struct sockaddr_in  *address = nullptr;
    
    std::vector<Socket> clients;

    
};


void serverDriverCode();

} //namespace shb
