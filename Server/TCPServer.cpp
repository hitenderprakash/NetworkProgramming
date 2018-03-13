/*
    Hitender Prakash
    subclass of Server for TCPServers
*/
#include "server.h"

TCPServer::TCPServer(int port, int domain, int con_limit){
    type=SOCK_STREAM;
    protocol=0;
    this->domain=domain;
    listen_port=port;
    bzero((char*)&serv_addr,sizeof(serv_addr));
    serv_addr.sin_addr.s_addr=INADDR_ANY;
    serv_addr.sin_port = htons(listen_port);
    listen_socketfd=socket(this->domain,type,protocol);
    connection_limit = con_limit;
    active_connections = 0;
}
int TCPServer::Bind(){
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

int TCPServer::Listen(){
    int res = listen(listen_socketfd,connection_limit);
    if (res<0){
        std::cerr<<"\nError: listen failed";
    }
    return res;
}

void TCPServer::Run(){
    while(1){
        struct sockaddr_in client_addr;
        socklen_t clilen=sizeof(client_addr);
        int nwsockfd;
        while((nwsockfd=accept(listen_socketfd, (struct sockaddr *)&client_addr,&clilen))>0){
            struct clientInfo client(client_addr,nwsockfd);
            //start new service thread only if number of active connections hhas not crossed the limit
            //otherwise drop the connection
            //this needs to be guarded by mutex
            mtx.lock();
            if(active_connections < connection_limit){
                std::thread servThread(&Server::serveRequest,this,client);
                active_connections++;
                servThread.detach();
            }
            else{
                //std::cout<<"\nConnection Limit over, can't accept new connection";
                //std::cout<<"\nDropping connection from: ";
                close(nwsockfd);
            }
            mtx.unlock();
        }
    }
    close(listen_socketfd);
}
