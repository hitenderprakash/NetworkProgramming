#include "server.h"

UDPServer::UDPServer(int port, int domain, int con_limit){
    type=SOCK_DGRAM;
    protocol=0;
    this->domain=domain;
    listen_port=port;
    bzero((char*)&serv_addr,sizeof(serv_addr));
    serv_addr.sin_addr.s_addr=INADDR_ANY;
    serv_addr.sin_port = htons(listen_port);
    listen_socketfd=socket(this->domain,type,protocol);
    connection_limit = con_limit;
}
int UDPServer::Bind(){
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

int UDPServer::Listen(){
    int res = listen(listen_socketfd,connection_limit);
    if (res<0){
        std::cerr<<"\nError: listen failed";
    }
    return res;
}

void UDPServer::Run(){
    while(1){
        struct sockaddr_in client_addr;
        socklen_t clilen=sizeof(client_addr);
        int nwsockfd;
        while((nwsockfd=accept(listen_socketfd, (struct sockaddr *)&client_addr,&clilen))>0){
            struct clientInfo client(client_addr,nwsockfd);
            std::thread servThread(&Server::serveRequest,this,client);
            servThread.detach();
        }
    }
    close(listen_socketfd);
}
