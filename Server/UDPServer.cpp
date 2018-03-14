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
    //UDP Server does not call listen but to keep the interface generic for all server we will return success
    return 1;
}

void UDPServer::serveRequest(clientInfo client){
    
}

void UDPServer::Run(){
    while(1){
        struct sockaddr_in client_addr;
        socklen_t clilen=sizeof(client_addr);
        int n;
        char buff[1024];
        bzero(buff,1024);
        n=recvfrom(listen_socketfd, buff,sizeof(buff),0,(struct sockaddr *)&client_addr,&clilen);
        if(n<0){
            std::cerr<<"\nError: UDP Receive failed";
        }
        //char *ipad = gethostbyaddr((const char *)&client_addr.sin_addr.s_addr,sizeof(client_addr.sin_addr.s_addr),domain);
        std::string ClientIP = std::string(inet_ntoa(client_addr.sin_addr));
        int clientPort = ntohs(client_addr.sin_port);
        std::cout<<"\nServing new connection from: "<<ClientIP<<" :"<<clientPort<<"\n";
        n = sendto(listen_socketfd, buff, n, 0,(struct sockaddr *)&client_addr, clilen);
    }
}
