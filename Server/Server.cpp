/*
    Hitender Prakash
    contains the general methods of server class which are common for both TCP and UDP server
*/
#include "server.h"

void Server::serveRequest(clientInfo client){
    int n;
    char buff[1024];
    bzero(buff,1024);
    int nwsockfd = client.socketfd;
    struct sockaddr_in cli_addr = client.addr;
    struct in_addr ipAddr = cli_addr.sin_addr;
    char ip[INET_ADDRSTRLEN];
    inet_ntop(AF_INET,&ipAddr,ip,INET_ADDRSTRLEN);

    cout<<"\nServing new connection from: "<<ip<<" :"<<ntohs(cli_addr.sin_port)<<"\n";

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
    close(nwsockfd);
}
