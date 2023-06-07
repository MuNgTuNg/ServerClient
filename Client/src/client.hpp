#pragma once
#include<iostream>
#include <vector>
#include <thread>
#include <unistd.h>

#include <stdio.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string.h>
#include <malloc.h>


/* 

                TODO:: 
                    » Look into the run() function
                    » Add IP details read from file functionality
*/

namespace shb{

class Client{
   public:
    Client(int portIN, const std::string& ipIN);
    ~Client();

    void run();
    void sendAndProcessData();
    void recieveAndProcessData();
        
    // struct ClientData{
    //     char sendBuffer[1024];
    //     char recvBuffer[1024];
    // };
    
    //ClientData data;
    
   private:
    
    int port = 0;
    std::string ip = "127.0.1.1";
    int clientFD;
    struct sockaddr_in  *address = nullptr;

    
    


};

void clientDriverCode(int, std::string);


} //namespace shb