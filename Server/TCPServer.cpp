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
    while(1){
        ConnectionInfo conInfo(domain,type,1024);
        //struct sockaddr_in client_addr;
        socklen_t clilen=sizeof(conInfo.ClientAddr);
        bzero(conInfo.buff,conInfo.buffSize);
        if((conInfo.socketfd = accept(listen_socketfd, (struct sockaddr *)&conInfo.ClientAddr,&clilen))>0){
            //start new service thread only if number of active connections hhas not crossed the limit
            //otherwise drop the connection
            //this needs to be guarded by mutex
            mtx.lock();
            if(active_connections < connection_limit){
                std::thread servThread(&TCPServer::serveRequest,this,conInfo);
                active_connections++;
                servThread.detach();
            }
            else{
                //std::cout<<"\nConnection Limit over, can't accept new connection";
                //std::cout<<"\nDropping connection from: ";
                close(conInfo.socketfd);
            }
            mtx.unlock();
        }
    }
    close(listen_socketfd);
}

//method for serving the request of the client
//to be executed by a new thread
void TCPServer::serveRequest(ConnectionInfo conInfo){
    std::string ClientIP = conInfo.getIPAddress();
    int clientPort = conInfo.getPort();

    std::cout<<"\nServing new connection from: "<<ClientIP<<" :"<<clientPort<<"\n";

    conInfo.numBytes=read(conInfo.socketfd,conInfo.buff,conInfo.buffSize);
    if(conInfo.numBytes<0){cerr<<"\nError reading from socket\n";}
    //get request
    std::string msg="";
    if(conInfo.buff){msg=std::string(conInfo.buff);}
    //cout<<msg;
    std::string fname=split(split(split(msg,"\r\n")[0]," ")[1],"/")[1];

    //since files are kept at different file path depending on their types
    //create the full file path
    //currently hard coding all the directory to "Files"
    std::string absFileName= "./Files/"+fname;
    //prepare response
    std::string response = "";
    string fileBuf="";
    ifstream resFile;
    resFile.open(absFileName.c_str());
    if(resFile){
        response="HTTP/1.0 200 OK\r\nContent-Type: text/html\r\nconnection: keep-alive\r\n\r\n";
        while(!resFile.eof()){
            std::string temp="";
            getline(resFile,temp);
            fileBuf+="\n"+temp;
        }
        resFile.close();
    }
    else{
        response="HTTP/1.0 404 NOT FOUND\r\nContent-Type: text/html\r\n\r\n";
    }
    send(conInfo.socketfd,response.c_str(),strlen(response.c_str()),0);
    write(conInfo.socketfd,fileBuf.c_str(),strlen(fileBuf.c_str())-1);
    mtx.lock();

    //close socket for this connection
    close(conInfo.socketfd);
    active_connections--;
    mtx.unlock();
}
