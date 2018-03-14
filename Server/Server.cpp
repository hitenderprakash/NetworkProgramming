/*
    Hitender Prakash
    contains the general methods of server class which are common for both TCP and UDP server
*/
#include "server.h"
//initialize common interface for a server
//call this function in constructor of derived classes
void Server::initCommonServerParam(int port, int domain, int con_limit){
    protocol=0;
    this->domain=domain;
    listen_port=port;
    connection_limit = con_limit;
    active_connections = 0;
    bzero((char*)&serv_addr,sizeof(serv_addr));
    serv_addr.sin_addr.s_addr=INADDR_ANY;
    serv_addr.sin_port = htons(listen_port);

    //delegate these to the derived class constructors
    //type = SOCK_STREAM or SOCK_DGRAM
    //listen_socketfd=socket(this->domain,type,protocol);
}
int Server::Bind(){
    int res=-1;
    if(listen_socketfd < 0){
        std::cout<<"\nError: Socket was not opened";
        return res;
    }
    res = bind(listen_socketfd,(struct sockaddr *)&serv_addr,sizeof(serv_addr));
    if(res<0){
        std::cerr<<"\nError: bind failed";
        return res;
    }
    return res;
}

int Server::Listen(){
    int res = listen(listen_socketfd,connection_limit);
    if (res<0){
        std::cerr<<"\nError: listen failed";
    }
    return res;
}
