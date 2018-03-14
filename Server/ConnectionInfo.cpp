/*

    Hitender Prakash
    Interface for client info - clientInfo object keep information of client and socket. It is passed to new thread which server the request
*/

#include "server.h"
//constructor
//for now I am not using the domain, type and buffsize
ConnectionInfo::ConnectionInfo(int domain, int type,int buffSize){
    this->domain = domain;
    this->type = type;
    socketfd = -1;
    numBytes = -1;
    //this->buffSize = buffSize;
    //buff = new char(buffSize);
}

//get socket on which the connection from client has been accepted
int ConnectionInfo::getSocket(){
    return socketfd;
}

//get printable IP Address of client
std::string ConnectionInfo::getIPAddress(){
    /*struct in_addr ipAddr = addr.sin_addr;
    char ip[INET_ADDRSTRLEN];
    inet_ntop(AF_INET,&ipAddr,ip,INET_ADDRSTRLEN);
    return std::string(ip);*/
    return (std::string(inet_ntoa(ClientAddr.sin_addr)));
}

//get the port of the client
int ConnectionInfo::getPort(){
    return ntohs(ClientAddr.sin_port);
}
