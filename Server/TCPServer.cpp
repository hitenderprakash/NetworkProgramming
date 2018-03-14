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

/*void TCPServer::Run(){
    Server::Run();
}*/

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
                std::thread servThread(&TCPServer::serveRequest,this,client);
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

//method for serving the request of the client
//to be executed by a new thread
void TCPServer::serveRequest(clientInfo client){
    int n;
    char buff[1024];
    bzero(buff,1024);
    int nwsockfd = client.getSocket();
    std::string ClientIP = client.getIPAddress();
    int clientPort = client.getPort();

    std::cout<<"\nServing new connection from: "<<ClientIP<<" :"<<clientPort<<"\n";

    n=read(nwsockfd,buff,1023);
    if(n<0){cerr<<"\nError reading from socket\n";}
    //get request
    std::string msg="";
    if(buff){msg=std::string(buff);}
    //cout<<msg;
    std::string fname=split(split(split(msg,"\r\n")[0]," ")[1],"/")[1];

    //since files are kept at different file path depending on their types
    //create the full file path
    //currently hard coding all the directory to "Files"
    std::string absFileName= "./Files/"+fname;
    //prepare response
    std::string reply = "";
    string fileBuf="";
    ifstream resFile;
    resFile.open(absFileName.c_str());
    if(resFile){
        reply="HTTP/1.0 200 OK\r\nContent-Type: text/html\r\nconnection: keep-alive\r\n\r\n";
        while(!resFile.eof()){
            std::string temp="";
            getline(resFile,temp);
            fileBuf+="\n"+temp;
        }
        resFile.close();
    }
    else{
        reply="HTTP/1.0 404 NOT FOUND\r\nContent-Type: text/html\r\n\r\n";
    }
    send(nwsockfd,reply.c_str(),strlen(reply.c_str()),0);
    write(nwsockfd,fileBuf.c_str(),strlen(fileBuf.c_str())-1);
    mtx.lock();
    close(nwsockfd);
    active_connections--;
    mtx.unlock();
}
