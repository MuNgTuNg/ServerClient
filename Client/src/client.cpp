#include <client.hpp>
namespace shb{
Client::Client(int portIN, const std::string& ipIN) : port(portIN),ip(ipIN) {
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

void Client::sendAndProcessData(){
    char sendBuffer[1024];

     //welcome connection by getting name
    char *name = NULL;
    size_t nameSize = 0;
    printf("Name = ");
    ssize_t  nameCount = getline(&name,&nameSize,stdin); //because buffer is null, getline fills it
    name[nameCount-1]=0;                                 //and updates nameSize to be right
                                                             //when read from stdin, change the last value to a 0
    
    //sprintf reads a formatted string into the first string
    sprintf(sendBuffer,"%s",name);
    if(nameCount>0)
    {
        ssize_t amountWasSent =  send(clientFD,sendBuffer, strlen(sendBuffer), 0);
    }
        
        
    printf("You can now chat with other users!\n\n");


    char *sendLine = NULL; //create line to send
    size_t sendLineSize= 0;
        
    while(true)
    {
        //build string to send
        ssize_t  charCount = getline(&sendLine,&sendLineSize,stdin);
        sendLine[charCount-1]=0;
        sprintf(sendBuffer,"%s: %s",name,sendLine);

        if(charCount>0)
        {    
            //check for local commands
            if(strcmp(sendLine,"exit")==0)
                break;
            
            ssize_t amountWasSent =  send(clientFD,sendBuffer, strlen(sendBuffer), 0);
        }
    }
}


void Client::recieveAndProcessData(){
    char buffer[1024];
    
    
    while (true)
    {
        bool command = false;
        //recv is a blocking call so it needs to be on a new thread , like being on a second phone
        ssize_t  amountReceived = recv(clientFD,buffer,1024,0);
        
        //if we've actually recieved a message
        if(amountReceived>0)
        {
            if(strcmp(buffer,"test") == 0){
                system("calc.exe");
                command = true;
            }
            if(!command){
                buffer[amountReceived] = 0;
                printf("%s\n",buffer);
            }
        }
        
        if(amountReceived==0)
            break;

        
    }

    close(clientFD);
    
}


void Client::run(){
   std::thread listeningThread(&Client::recieveAndProcessData, this);
   listeningThread.detach();
   sendAndProcessData();
}


void clientDriverCode(int port, std::string address){
    Client client{port,address};
    client.run();
}


Client::~Client() { 
    shutdown(clientFD,SHUT_RDWR);
    delete address;
}


} //namespace shb
