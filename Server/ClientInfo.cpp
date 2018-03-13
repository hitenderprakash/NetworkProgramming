/*

    Hitender Prakash
    Interface for client info - clientInfo object keep information of client and socket. It is passed to new thread which server the request
*/

#include "server.h"

//get socket on which the connection from client has been accepted
int clientInfo::getSocket(){
    return socketfd;
}

//get printable IP Address of client
std::string clientInfo::getIPAddress(){
    struct in_addr ipAddr = addr.sin_addr;
    char ip[INET_ADDRSTRLEN];
    inet_ntop(AF_INET,&ipAddr,ip,INET_ADDRSTRLEN);
    return std::string(ip);
}

//get the port of the client
int clientInfo::getPort(){
    return ntohs(addr.sin_port);
}
