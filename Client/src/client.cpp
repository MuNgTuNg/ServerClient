#include <client.hpp>
namespace shb{


void Client::run(){

        char *name = NULL;
        size_t nameSize= 0;
        printf("please enter your name?\n");
        ssize_t  nameCount = getline(&name,&nameSize,stdin);
        name[nameCount-1]=0;


        char *line = NULL;
        size_t lineSize= 0;
        printf("type and we will send(type exit)...\n");


        char buffer[1024];

        while(true)
        {


            ssize_t  charCount = getline(&line,&lineSize,stdin);
            line[charCount-1]=0;

            sprintf(buffer,"%s:%s",name,line);

            if(charCount>0)
            {
                if(strcmp(line,"exit")==0)
                    break;

                ssize_t amountWasSent =  send(clientFD,
                                              buffer,
                                              strlen(buffer), 0);
            }
        }
       
}


void clientDriverCode(){
    Client client{8091,"127.0.1.1"};

    client.run();
}



} //namespace shb