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


/* 

                TODO:: 
                    » Look into the run() function
                    » Add IP details read from file functionality
*/

namespace shb{

class Client{
   public:
    Client(int portIN, const std::string& ipIN) : port(portIN),ip(ipIN) {
        //sock stream connection
    
        clientFD = socket(AF_INET,SOCK_STREAM,0);

        //set up the address
        address = (struct sockaddr_in*)malloc(sizeof(struct sockaddr_in));
        address->sin_family = AF_INET;
        address->sin_port = htons(port);

        if(strlen(ip.c_str()) ==0) //if no IP chosen just use any
            address->sin_addr.s_addr = INADDR_ANY;
        else //convert IP address to binary
            inet_pton(AF_INET,ip.c_str(),&address->sin_addr.s_addr);
        

        //connect socket to the address
        int res = connect(clientFD,(const sockaddr*)address,sizeof(*address));
        
        if(res != 0){
            printf("Client connect unsuccessful\n");
        }
        else{
            printf("Connection successful!\n");
        }
    }


    void run();
        
    
    
   private:
    int port = 8088;
    std::string ip = "127.0.1.1";
    int clientFD;
    struct sockaddr_in  *address = nullptr;
    


};

void clientDriverCode();


} //namespace shb