/*
    Hitender Prakash
    subclass of Server for TCPServers
*/
#include "server.h"

TCPServer::TCPServer(int port, int domain, int con_limit){
    //init common parameters of server
    Server::initCommonServerParam(port,domain,con_limit);
    //set type
    type=SOCK_STREAM;
    //create TCP socket
    listen_socketfd=socket(this->domain,type,protocol);
}

int TCPServer::Bind(){
    return Server::Bind();
}

int TCPServer::Listen(){
    return Server::Listen();
}

void TCPServer::Run(){
    Server::Run();
}
