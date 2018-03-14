
#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <sstream>
#include <iterator>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
using namespace std;

int main(int argc, char *argv[]){
    int sockfd;
    int port;
    int n;
    struct sockaddr_in serv_addr;
    struct hostent *server;

    char buff[1024];
    if(argc<3){
        std::cerr<<"\nUsage: ./UDPClient <hostname> <port>";
        exit(0);
    }

    try{
        port=stoi(argv[2]);
        if(port<=0 || port > 65535){
            throw;
        }
    }
    catch(...){
        std::cerr<<"\nError: Invalid port entered";
        exit(0);
    }
    sockfd=socket(AF_INET,SOCK_DGRAM,0);
    if(sockfd<0){
        std::cerr<<"\nError: in opening socket";
        exit(0);
    }
    server=gethostbyname(argv[1]);
    if(server==NULL){
        std::cerr<<"\nError: No such Host exists";
        exit(0);
    }
    bzero((char*)&serv_addr,sizeof(serv_addr));
    serv_addr.sin_family=AF_INET;
    bcopy((char*)server->h_addr,(char *)&serv_addr.sin_addr.s_addr,server->h_length);
    serv_addr.sin_port=htons(port);

    if(connect(sockfd,(struct sockaddr *)&serv_addr,sizeof(serv_addr))<0){
        std::cerr<<"\nError in connecting to host";
        exit(0);
    }


    bzero(buff,1024);
    cout<<"\nPlease enter the file name: ";
    string fname="";
    cin>>fname;
    string req=fname;
    strcpy(buff,req.c_str());
    //fgets(buff,1024,stdin);
    n=write(sockfd,buff,strlen(buff));
    if(n<0){
        fprintf(stderr,"\nError in writing to socket");
        exit(0);
    }
    bzero(buff,1024);
    n=read(sockfd,buff,1023);
    if(n<0){
        fprintf(stderr,"\nError in reading from socket");
        exit(0);
    }
    std::cout<<"\nResponse from server:";
    std::cout<<buff;
    std::cout<<"\n";
    close(sockfd);
    return 0;
}
