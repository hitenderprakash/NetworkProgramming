/*
    Hitender Prakash
    subclass of Server for UDPServers
*/
#include "server.h"


UDPServer::UDPServer(int port, int domain, int con_limit){
    //init common parameters of server
    Server::initCommonServerParam(port,domain,con_limit);
    //set type
    type=SOCK_DGRAM;
    //create UDP socket
    listen_socketfd=socket(this->domain,type,protocol);
}

int UDPServer::Bind(){
    return Server::Bind();
}

int UDPServer::Listen(){
    return Server::Listen();
}

void UDPServer::Run(){
    Server::Run();
}
